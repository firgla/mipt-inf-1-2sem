#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <random>
#include <unistd.h>
#include <algorithm>
#include <string> 

template<typename T>
using Vec_1d = std::vector<T>;

template<typename T>
using Vec_2d= std::vector<std::vector<T>>;

template<typename T>
using Vec_3d = std::vector<std::vector<std::vector<T>>>;

#define float double

const float PI = std::acos(-1.0);

class Particle
{
private:
    float m;
    Vec_1d<float> velocity = Vec_1d<float>(3);
    Vec_1d<float> position = Vec_1d<float>(3);

public:
    Particle(Vec_1d<float> position, Vec_1d<float> velocity, float m): m(m), velocity(velocity), position(position)
    {}

    float distance(Particle& other)
    {
        return std::sqrt(std::pow(this->position[0]-other.position[0], 2) +
                         std::pow(this->position[1]-other.position[1], 2) +
                         std::pow(this->position[2]-other.position[2], 2));
    }

    float W(Particle& other, float h)
    {
        float abs_r = distance(other);
        return 1 / (std::pow(h, 3) + std::pow(PI,1.5))
               * std::exp(-std::pow(abs_r,2)/std::pow(h,2));
    }

    Vec_1d<float> gradW(Particle& other, float h)
    {
        float abs_r = distance(other);
        float scalar_deriv = -2 / (std::pow(h, 5) * std::pow(PI, 1.5))
                             * std::exp(-std::pow(abs_r, 2) / std::pow(h, 2));
        Vec_1d<float> grad = {scalar_deriv * (this->position[0] - other.position[0]),
                              scalar_deriv * (this->position[1] - other.position[1]),
                              scalar_deriv * (this->position[2] - other.position[2])};
        return grad;
    }

    float density(Vec_1d<Particle>& particles, float h) 
    {
        int N = particles.size();
        Vec_1d<float> R_ij(2);
        float rho = 0;
        for (int j = 0; j < N; j++) 
        {
            float rho_ij = particles[j].m * this->W(particles[j], h);
            rho += rho_ij;
        }

        return rho;
    }

    float pressure(float density, float k, float n) 
    {
        float p;
        p = k * std::pow(density, 1 + 1 / n);
        return p;
    }

    Vec_1d<float> get_position()
    {
        return position;
    }

    Vec_1d<float> get_velocity()
    {
        return velocity;
    }

    float get_m()
    {
        return m;
    }

    void change_velocity(Vec_1d<float>& acc, float dt)
    {
        for (int k = 0; k < 3; k ++)
        {
            velocity[k] += acc[k] * dt / 2;
        }
    }

    void change_position(Vec_1d<float> velocity, float dt)
    {
        for (int k = 0; k < 3; k ++)
        {
            position[k] += velocity[k] * dt;
        }
    }
};

Vec_2d<float> acceleration(Vec_1d<Particle> particles, float h, float k, float n, float lmbda, float nu) 
{
    int N = particles.size();
    Vec_2d<float> a(N, Vec_1d<float>(2));
    Vec_1d<float> rho (N);
    Vec_1d<float> p (N);

    for (int i = 0; i < N; i++)
    {
        rho[i] = particles[i].density(particles, h);
        p[i] = particles[i].pressure(rho[i], k, n);
    }

    Vec_1d<float> R_ij(2);
    Vec_1d<float> a_p(2);

    for (int i = 0; i < N; i++) 
    {
        a[i][0] += - nu * particles[i].get_velocity()[0] - lmbda * particles[i].get_position()[0];
        a[i][1] += - nu * particles[i].get_velocity()[1] - lmbda * particles[i].get_position()[1];
        for (int j = i + 1; j < N; j++) 
        {
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

Vec_2d<Particle> calc(Vec_1d<Particle>& particles, float h, float k, float n, float lmbda, float nu, float tmax, float dt)
{
    int N = particles.size();
    int Nt = tmax / dt;
    Vec_2d<Particle> result (Nt);

    for(int i = 0; i < N; i ++)
    {
        result[0].push_back(particles[i]);
    }

    Vec_2d<float> acc = acceleration(particles, h, k, n, lmbda, nu);
    Vec_1d<float> rho (N);

    for (int i = 1; i < Nt; i ++)
    {
        for (int j = 0; j < N; j++)
        {
            particles[j].change_velocity(acc[j], dt);
            particles[j].change_position(particles[j].get_velocity(), dt);
            acc = acceleration(particles, h, k, n, lmbda, nu);
            particles[j].change_velocity(acc[j], dt);
        }

        for(int j = 0; j < N; j ++)
        {
            result[i].push_back(particles[j]);
        }
    }

    return result;
}

sf::Color hsv(int hue, float sat, float val)
{
    hue %= 360;
    while(hue < 0) hue += 360;

    if(sat < 0.f) sat = 0.f;
    if(sat > 1.f) sat = 1.f;

    if(val < 0.f) val = 0.f;
    if(val > 1.f) val = 1.f;

    int h = hue / 60;
    float f = float(hue) / 60 - h;
    float p = val * (1.f - sat);
    float q = val * (1.f - sat * f);
    float t = val * (1.f - sat * (1 - f));

    switch(h)
    {
        default:
        case 0:
        case 6: return sf::Color(val * 255, t * 255, p * 255);
        case 1: return sf::Color(q * 255, val * 255, p * 255);
        case 2: return sf::Color(p * 255, val * 255, t * 255);
        case 3: return sf::Color(p * 255, q * 255, val * 255);
        case 4: return sf::Color(t * 255, p * 255, val * 255);
        case 5: return sf::Color(val * 255, p * 255, q * 255);
    }
}

int main()
{
    const int N_part = 100;
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

    std::size_t step = 0;

    std::vector <float> rhos;
    rhos.clear();

    for (std::size_t i = 0; i < result.size(); i++)
    {
        for(std::size_t j = 0; j < result[i].size(); j++)
        {
            float rho = result[i][j].density(result[i], h);
            rhos.push_back(rho);
        }
    }

    sf::RenderWindow window (sf::VideoMode (600, 600), "Toy Star Simulation");
    float want_fps = 60;
    sf::Clock loop_timer;

    auto it = std::minmax_element(rhos.begin(), rhos.end(), std::greater<float>());
    auto max_rho = *it.first;
    auto min_rho = *it.second;

    float rhos_range = max_rho - min_rho;
    float rhos_step = rhos_range / 360;

    float rhos_quarter = min_rho + 360 * rhos_step / 4;
    float rhos_half = rhos_quarter + 180 * rhos_step / 2;

    std::sort(rhos.begin(), rhos.end() - 1);


    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
       
        for (int i = 0; i < sprites.size(); i++)
        {
            sprites[i].setRadius(2);
            sprites[i].setOutlineThickness(1.5);

            for(std::size_t j = 0; j < result[step].size(); j++)
            {
                sprites[i].setPosition(result[step][j].get_position()[0] * 100 + 300, result[step][j].get_position()[1] * 100 + 300);
                float rho = result[step][j].density(result[step], h);
                if (rho < rhos_quarter)
                {
                    for (int k = 1; k < 360; k++)
                    {   
                        if (min_rho + (k - 1) * rhos_step / 4 <= rho && rho < min_rho + k * rhos_step / 4)
                        {
                            sprites[i].setFillColor(hsv(k, 1.f, 0.5));
                            sprites[i].setOutlineColor(hsv(k, 1.f, 1.f));
                        }
                    }   
                }

                else if (rho < rhos_half)
                {
                    for (int k = 1; k < 180; k++)
                    {   
                        if (rhos_quarter + (k - 1) * rhos_step / 2 <= rho && rho < rhos_quarter + k * rhos_step / 2)  
                        {      
                            sprites[i].setFillColor(hsv(k, 1.f, 0.5));
                            sprites[i].setOutlineColor(hsv(k, 1.f, 1.f));
                        }
                    } 
                }

                else 
                { 
                    for (int k = 1; k < 180; k++)
                    {
                        if (rhos_half + (k - 1) * rhos_step <= rho && rho < rhos_half + k * rhos_step)   
                        {     
                            sprites[i].setFillColor(hsv(k, 1.f, 0.5));
                            sprites[i].setOutlineColor(hsv(k, 1.f, 1.f));
                        }
                    }
                }

                window.draw(sprites[i]);
            }
        }


//шкала

        sf::VertexArray quad1(sf::Quads, 4);
        sf::VertexArray quad2(sf::Quads, 4);
        sf::VertexArray quad3(sf::Quads, 4);


        quad1[0].position = sf::Vector2f(560.f, 155.f);
        quad1[1].position = sf::Vector2f(590.f, 155.f);
        quad1[2].position = sf::Vector2f(590.f, 255.f);
        quad1[3].position = sf::Vector2f(560.f, 255.f);

        quad2[0].position = sf::Vector2f(560.f, 255.f);
        quad2[1].position = sf::Vector2f(590.f, 255.f);
        quad2[2].position = sf::Vector2f(590.f, 350.f);
        quad2[3].position = sf::Vector2f(560.f, 350.f);

        quad3[0].position = sf::Vector2f(560.f, 350.f);
        quad3[1].position = sf::Vector2f(590.f, 350.f);
        quad3[2].position = sf::Vector2f(590.f, 445.f);
        quad3[3].position = sf::Vector2f(560.f, 445.f);


        quad1[0].color = sf::Color{255, 0, 255, 255};
        quad1[1].color = sf::Color{255, 0, 255, 255};
        quad1[2].color = sf::Color{0, 255, 255, 255};
        quad1[3].color = sf::Color{0, 255, 255, 255};

        quad2[0].color = sf::Color{0, 255, 255, 255};
        quad2[1].color = sf::Color{0, 255, 255, 255};
        quad2[2].color = sf::Color{255, 255, 0, 255};
        quad2[3].color = sf::Color{255, 255, 0, 255};

        quad3[0].color = sf::Color{255, 255, 0, 255};
        quad3[1].color = sf::Color{255, 255, 0, 255};
        quad3[2].color = sf::Color{255, 0, 0, 255};
        quad3[3].color = sf::Color{255, 0, 0, 255};

//линии для контура шкалы

        sf::Vertex line1[] =
        {
            sf::Vertex(sf::Vector2f(560.f, 155.f)),
            sf::Vertex(sf::Vector2f(590.f, 155.f))
        };

        sf::Vertex line2[] =
        {
            sf::Vertex(sf::Vector2f(590.f, 155.f)),
            sf::Vertex(sf::Vector2f(590.f, 445.f))
        };

        sf::Vertex line3[] =
        {
            sf::Vertex(sf::Vector2f(590.f, 445.f)),
            sf::Vertex(sf::Vector2f(560.f, 445.f))
        };

        sf::Vertex line4[] =
        {
            sf::Vertex(sf::Vector2f(560.f, 445.f)),
            sf::Vertex(sf::Vector2f(560.f, 155.f))
        };


        sf::Text text_min;
        sf::Text text_max;
        sf::Text text_half;
        sf::Text text_quarter;
        sf::Text text_density;

        sf::Font font;
        font.loadFromFile("font.ttf");

        text_min.setFont(font);
        text_max.setFont(font); 
        text_half.setFont(font); 
        text_quarter.setFont(font);  
        text_density.setFont(font);

        std::string min_str = std::to_string(min_rho);
        std::string max_str = std::to_string(max_rho);
        std::string half_str = std::to_string(rhos_half);
        std::string quarter_str = std::to_string(rhos_quarter);

        text_min.setString(min_str);
        text_max.setString(max_str);
        text_half.setString(half_str);
        text_quarter.setString(quarter_str);
        text_density.setString("Density");

        int size = 12;

        text_min.setCharacterSize(size);
        text_max.setCharacterSize(size); 
        text_half.setCharacterSize(size); 
        text_quarter.setCharacterSize(size);  
        text_density.setCharacterSize(20);


        text_min.setFillColor(sf::Color::White);
        text_max.setFillColor(sf::Color::White);
        text_half.setFillColor(sf::Color::White);
        text_quarter.setFillColor(sf::Color::White);
        text_density.setFillColor(sf::Color::White);

        text_min.setPosition(560.f - 7 * min_str.length(), 445.f - size / 2);
        text_max.setPosition(560.f - 7 * max_str.length(), 155.f - size / 2);
        text_half.setPosition(560.f - 7 * half_str.length(), 300.f - size / 2);
        text_quarter.setPosition(560.f - 7 * quarter_str.length(), 372.5 - size / 2);
        text_density.setPosition((590.f + (560.f - 7 * max_str.length())) / 2 - 25.f, 120.f);


        sf::Vertex line5[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 300.f)),
            sf::Vertex(sf::Vector2f(590.f, 300.f))
        };

        sf::Vertex line6[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 372.5)),
            sf::Vertex(sf::Vector2f(590.f, 372.5))
        };

        sf::Vertex line7[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 155.f)),
            sf::Vertex(sf::Vector2f(590.f, 155.f))
        };

        sf::Vertex line8[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 445.f)),
            sf::Vertex(sf::Vector2f(590.f, 445.f))
        };

        sf::Vertex line9[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 299.f)),
            sf::Vertex(sf::Vector2f(590.f, 299.f))
        };

        sf::Vertex line10[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 371.5)),
            sf::Vertex(sf::Vector2f(590.f, 371.5))
        };

        sf::Vertex line11[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 154.f)),
            sf::Vertex(sf::Vector2f(590.f, 154.f))
        };

        sf::Vertex line12[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 444.f)),
            sf::Vertex(sf::Vector2f(590.f, 444.f))
        };

        sf::Vertex line13[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 301.f)),
            sf::Vertex(sf::Vector2f(590.f, 301.f))
        };
        

        sf::Vertex line14[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 373.5)),
            sf::Vertex(sf::Vector2f(590.f, 373.5))
        };

        sf::Vertex line15[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 156.f)),
            sf::Vertex(sf::Vector2f(590.f, 156.f))
        };

        sf::Vertex line16[] =
        {
            sf::Vertex(sf::Vector2f(558.f, 446.f)),
            sf::Vertex(sf::Vector2f(590.f, 446.f))
        };

/*
        float graphic_step = 290 / 360;

        sf::Vertex line17[] =
        {
            sf::Vertex(sf::Vector2f(560.f, 445.f - graphic_step / 4)),
            sf::Vertex(sf::Vector2f(590.f, 445.f - graphic_step / 4))
        };

        sf::Vertex line18[] =
        {
            sf::Vertex(sf::Vector2f(560.f, 445.f - graphic_step / 2)),
            sf::Vertex(sf::Vector2f(590.f, 445.f - graphic_step / 2))
        };
        sf::Vertex line19[] =
        {
            sf::Vertex(sf::Vector2f(560.f, 372.5 - graphic_step / 2)),
            sf::Vertex(sf::Vector2f(590.f, 372.5 - graphic_step / 2))
        };

        sf::Vertex line20[] =
        {
            sf::Vertex(sf::Vector2f(560.f, 372.5 - graphic_step)),
            sf::Vertex(sf::Vector2f(590.f, 372.5 - graphic_step))
        };

        sf::Vertex line21[] =
        {
            sf::Vertex(sf::Vector2f(560.f, 300.f - graphic_step)),
            sf::Vertex(sf::Vector2f(590.f, 300.f - graphic_step))
        };

        sf::Vertex line22[] =
        {
            sf::Vertex(sf::Vector2f(560.f, 300.f - 2 * graphic_step)),
            sf::Vertex(sf::Vector2f(590.f, 300.f - 2 * graphic_step))
        };

*/
       
        window.draw(text_min);
        window.draw(text_max);
        window.draw(text_half);
        window.draw(text_quarter);
        window.draw(text_density);

        window.draw(quad1);
        window.draw(quad2);
        window.draw(quad3);
        window.draw(line1, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);
        window.draw(line3, 2, sf::Lines);
        window.draw(line4, 2, sf::Lines);
        window.draw(line5, 2, sf::Lines);
        window.draw(line6, 2, sf::Lines);
        window.draw(line7, 2, sf::Lines);
        window.draw(line8, 2, sf::Lines);
        window.draw(line9, 2, sf::Lines);
        window.draw(line10, 2, sf::Lines);
        window.draw(line11, 2, sf::Lines);
        window.draw(line12, 2, sf::Lines);
        window.draw(line13, 2, sf::Lines);
        window.draw(line14, 2, sf::Lines);
        window.draw(line15, 2, sf::Lines);
        window.draw(line16, 2, sf::Lines);

/*
        window.draw(line17, 2, sf::Lines);
        window.draw(line18, 2, sf::Lines);
        window.draw(line19, 2, sf::Lines);
        window.draw(line20, 2, sf::Lines);
        window.draw(line21, 2, sf::Lines);
        window.draw(line22, 2, sf::Lines);
*/

        window.display();

        if (step + 1  < result.size())
            step += 1;
        else
            step = 0;

        sf::Int32 frame_duration = loop_timer.getElapsedTime().asMilliseconds();
        sf::Int32 time_to_sleep = int(1000.f/want_fps) - frame_duration;

        if (time_to_sleep > 0) 
        {
            sf::sleep(sf::milliseconds(time_to_sleep));
        }

        loop_timer.restart();
    }

}