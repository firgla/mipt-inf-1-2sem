#include <SFML/Graphics.hpp>
#include<vector>
#include<iostream>
#include<cmath>
#include<iomanip>
#include <random>
#include<unistd.h>
#include<algorithm>

template<typename T>
using Vec_1d = std::vector<T>;
template<typename T>
using Vec_2d = std::vector<std::vector<T>>;
template<typename T>
using Vec_3d = std::vector<std::vector<std::vector<T>>>;

#define float double

const float PI = std::acos(-1.0);;

class Particle{
private:
    float m;
    Vec_1d<float> velocity =Vec_1d<float>(3);
    Vec_1d<float> position = Vec_1d<float>(3);

public:
    Particle(Vec_1d<float> position, Vec_1d<float> velocity, float m): m(m), velocity(velocity), position(position){
    }

    float distance(Particle& other){
        return std::sqrt(std::pow(this->position[0]-other.position[0], 2) +
                         std::pow(this->position[1]-other.position[1], 2) +
                         std::pow(this->position[2]-other.position[2], 2));
    }
    float W(Particle& other, float h){
        float abs_r = distance(other);
        return 1 / (std::pow(h, 3) + std::pow(PI,1.5))
                * std::exp(-std::pow(abs_r,2)/std::pow(h,2));
    }

    Vec_1d<float> gradW(Particle& other, float h){
        float abs_r = distance(other);
        float scalar_deriv = -2 / (std::pow(h, 5) * std::pow(PI, 1.5))
                                * std::exp(-std::pow(abs_r, 2) / std::pow(h, 2));
        Vec_1d<float> grad = {scalar_deriv * (this->position[0] - other.position[0]),
                              scalar_deriv * (this->position[1] - other.position[1]),
                              scalar_deriv * (this->position[2] - other.position[2])};
        return grad;
    }
    float density(Vec_1d<Particle>& particles, float h) {
        int N = particles.size();
        Vec_1d<float> R_ij(2);
        float rho = 0;
        for (int j = 0; j < N; j++) {
            float rho_ij = particles[j].m * this->W(particles[j], h);
            rho += rho_ij;
        }
        return rho;
    }

    float pressure(float density, float k, float n) {
        float p;
        p = k * std::pow(density, 1 + 1 / n);
        return p;
    }

    Vec_1d<float> get_position(){
        return position;
    }

    Vec_1d<float> get_velocity(){
        return velocity;
    }

    float get_m(){
        return m;
    }

    void change_velocity(Vec_1d<float>& acc, float dt){
        for (int k = 0; k < 3; k ++){
            velocity[k] += acc[k] * dt / 2;
        }
    }

    void change_position(Vec_1d<float> velocity, float dt){
        for (int k = 0; k < 3; k ++){
            position[k] += velocity[k] * dt;
        }
    }
};

Vec_2d<float> acceleration(Vec_1d<Particle> particles, float h, float k, float n, float lmbda, float nu) {
    int N = particles.size();
    Vec_2d<float> a(N, Vec_1d<float>(2));
    Vec_1d<float> rho (N);
    Vec_1d<float> p (N);
    for (int i = 0; i < N; i++){
        rho[i] = particles[i].density(particles, h);
        p[i] = particles[i].pressure(rho[i], k, n);
    }
    Vec_1d<float> R_ij(2);
    Vec_1d<float> a_p(2);
    for (int i = 0; i < N; i++) {
        a[i][0] += - nu * particles[i].get_velocity()[0] - lmbda * particles[i].get_position()[0];
        a[i][1] += - nu * particles[i].get_velocity()[1] - lmbda * particles[i].get_position()[1];
        for (int j = i + 1; j < N; j++) {
            a_p[0] = - particles[i].get_m() * (p[i] / pow(rho[i], 2) + p[j] / pow(rho[j], 2)) * particles[i].gradW(particles[j], h)[0];
            a_p[1] = - particles[i].get_m() * (p[i] / pow(rho[i], 2) + p[j] / pow(rho[j], 2)) * particles[i].gradW(particles[j], h)[1];
            a[i][0] += a_p[0];
            a[i][1] += a_p[1];
            a[j][0] += -a_p[0];
            a[j][1] += -a_p[1];
            }
    }
    return a;
}

Vec_2d<Particle> calc(Vec_1d<Particle>& particles, float h, float k, float n, float lmbda, float nu, float tmax, float dt){
    int N = particles.size();
    int Nt = tmax / dt;
    Vec_2d<Particle> result (Nt);
    for(int i = 0; i < N; i ++){
        result[0].push_back(particles[i]);
    }
    Vec_2d<float> acc = acceleration(particles, h, k, n, lmbda, nu);
    Vec_1d<float> rho (N);
    for (int i = 0; i < Nt; i ++){
        for (int j = 0; j < N; j++){
            particles[j].change_velocity(acc[j], dt);
            particles[j].change_position(particles[j].get_velocity(), dt);
            acc = acceleration(particles, h, k, n, lmbda, nu);
            particles[j].change_velocity(acc[j], dt);
        }
        for(int j = 0; j < N; j ++){
            result[i].push_back(particles[j]);
        }
    }
    return result;
}
sf::Color hsv(int hue, float sat, float val)
{
  hue %= 360;
  while(hue<0) hue += 360;

  if(sat<0.f) sat = 0.f;
  if(sat>1.f) sat = 1.f;

  if(val<0.f) val = 0.f;
  if(val>1.f) val = 1.f;

  int h = hue/60;
  float f = float(hue)/60-h;
  float p = val*(1.f-sat);
  float q = val*(1.f-sat*f);
  float t = val*(1.f-sat*(1-f));

  switch(h)
  {
    default:
    case 0:
    case 6: return sf::Color(val*255, t*255, p*255);
    case 1: return sf::Color(q*255, val*255, p*255);
    case 2: return sf::Color(p*255, val*255, t*255);
    case 3: return sf::Color(p*255, q*255, val*255);
    case 4: return sf::Color(t*255, p*255, val*255);
    case 5: return sf::Color(val*255, p*255, q*255);
  }
}

int main()
{
    const int N_part = 50;
    float h = 0.1;

        std::vector <sf::CircleShape> sprites(N_part);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> NormRand(0, 0.1);

        Vec_1d<Particle> particles;
        for (int i = 0; i < N_part; i++) 
        {
            particles.push_back(Particle(Vec_1d<float>{NormRand(gen), NormRand(gen), 0}, Vec_1d<float>{NormRand(gen),  NormRand(gen), 0}, 0.03));
        }

    float k = 0.1, n = 1, nu = 1, lmbda = 4, dt = 0.04, maxt = 6;
    Vec_2d<float> a(N_part, Vec_1d<float>(2));
    Vec_2d<Particle> result = calc(particles, h, k, n, lmbda, nu, maxt, dt);

    std::cout << result[0].size() << '\n';

    std::size_t step = 0;
    std::vector <float> rhos;

    for (sf::CircleShape &sprite : sprites) 
    {
            for(std::size_t j = 0; j < result[step].size(); j++)
            {
                float rho = result[step][j].density(result[step], h);
                rhos.push_back(rho);
            }
    }


    auto it = std::minmax_element(rhos.begin(), rhos.end(), std::greater<float>());
    float min_rho = *it.first;
    float max_rho = *it.second;

    float rhos_range = max_rho - min_rho;
    std::cout << result[step].size() << std::endl;
/*
    std::cout << "Vector before sort" << std::endl;
    for (float rho : rhos) 
    std::cout << rho << std::endl;
*/
/*
    std::cout << "--------------" << std::endl;
    std::sort(rhos.begin(), rhos.end() - 1);
 */
    float rhos_step = rhos_range / N_part;


    std::cout << min_rho << " " << max_rho << std::endl;
    std::cout << "--------------" << std::endl;
/*
    std::cout << "Vector after sort" << std::endl;
    for (float rho : rhos) 
    std::cout << rho << std::endl;

*/
    sf::RenderWindow window (sf::VideoMode (400, 400), " SFML works!");
    float want_fps = 5;
        sf::Clock loop_timer;
        while (window.isOpen()) {
          sf::Event event;
          while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
              window.close();
          }
          window.clear();
          for (sf::CircleShape &sprite : sprites) 
          {
            sprite.setRadius(1);
            for(std::size_t j = 0; j < result[step].size(); j++)
            {
              sprite.setPosition(result[step][j].get_position()[0] * 100 + 200, result[step][j].get_position()[1] * 60 + 200);
              float rho = result[step][j].density(result[step], h);
              for (int i = 1; i < 51; i++)
              {
                if (min_rho + (i - 1) * rhos_step <= rho < min_rho + i * rhos_step)
                    sprite.setFillColor(hsv(100 + i * 7, 1.f, 1.f));
              }
           
           
            /*
              if (min_rho < rho <= min_rho + rhos_range / 3125)
              {
                    sprite.setFillColor(sf::Color::White);
              }
              else if (min_rho + rhos_range / 3125 < rho <= min_rho + rhos_range / 625)
              {
                    sprite.setFillColor(sf::Color::Blue);
              }
              else if (min_rho + rhos_range / 625 < rho <= min_rho + rhos_range / 125)
              {
                    sprite.setFillColor(sf::Color::Green);
              }
              else if (min_rho + rhos_range / 125 < rho <= min_rho + rhos_range / 25)
              {
                    sprite.setFillColor(sf::Color::Yellow);
              }
              else if (min_rho + rhos_range / 25 < rho <= max_rho)
              {
                    sprite.setFillColor(sf::Color::Red);
              }
            */

              window.draw(sprite);
              /*
              std::cout << rho << std::endl;
              */
            }
          }
            window.display();
            if (step + 1 < result.size())
                step += 1;
            else
                step = 0;
            sf::Int32 frame_duration = loop_timer.getElapsedTime().asMilliseconds();
        sf::Int32 time_to_sleep = int(1000.f/want_fps) - frame_duration;
        if (time_to_sleep > 0) {
            sf::sleep(sf::milliseconds(time_to_sleep));
        }
        loop_timer.restart();
        }
        rhos.clear();
}