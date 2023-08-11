/*
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>


1 задача
int main()
{

    std::vector<int> summ;
    std::vector<int> summ_word;
    std::vector<std::string> text;
    int sum = 0;
    std::string sent;
        
    while(std::getline(std::cin, sent))
        text.push_back(sent);


    for(unsigned int j = 0; j < text.size(); j++)
    {

        for (unsigned int i = 0; i < text[j].length(); i++)
        {

            if (text[j][i] == 'l' || text[j][i] == 'i' || text[j][i] == 'g' || text[j][i] == 'h' || text[j][i] == 't' || text[j][i] == 'd' || text[j][i] == 'a' || text[j][i] == 'r' || text[j][i] == 'k')
            {
                sum += 1;
            }

            if(text[j][i] == ' ')
            {
                summ_word.push_back(sum);
                sum = 0;
            }
            
        }

        summ_word.push_back(sum);
        sum = 0;

        summ.push_back(summ_word[std::max_element(summ_word.begin(), summ_word.end()) - summ_word.begin()]);
        summ_word.clear();

    }
       
    int element = std::max_element(summ.begin(), summ.end()) - summ.begin();
    for (unsigned l = 0; l < text.size(); l ++)
        text[l].pop_back();
    
    std::cout << text[element] << std::endl;
    
}
*/

/*
2 задача
#include <iostream>
#include <cmath>
class Point
{
protected:
    int x, y;
public:
    Point(int x, int y): x(x), y(y) {} // конструктор
    int getX() const {return x;}
    int getY() const {return y;}
    Point operator+(const Point& a) const { // сложили две точки, вернули результат
        return Point(x + a.x, y + a.y);
    }
    Point operator-(const Point& a) const { // вычли две точки, вернули результат
        return Point(x - a.x, y - a.y);
    }
    Point operator*(double c) const { // умножили точку справа на коэффициент, вернули результат
        return Point(x * c, y * c);
    }
    double dist() const {
        return sqrt(x * x + y * y);
    }
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << "; " << p.y << ")";
    return os;
}

class AABB
{
private:
Point rt, lb;
public:
    AABB(const Point& lb, const Point& rt)
    : lb(lb), rt(rt)
    {}

    bool correct() const  
    {
        return (rt.getX() >= lb.getX() && rt.getY() >= lb.getY());
    }

    double diameter() const
    {
        return (rt - lb).dist();
    }

    bool contains(const Point& p) const
    {
        return (p.getX() <= rt.getX() && p.getX() >= lb.getX() && p.getY() <= rt.getY() && p.getY() >= lb.getY());
    }
    bool contains(const AABB& ab) const
    {
        return (ab.rt.getX() <= rt.getX() && ab.rt.getY() <= rt.getY() && ab.lb.getX() >= lb.getX() && ab.lb.getY() >= lb.getY());
    }
};

*/

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

int main()
{
    int N;
    std::cin >> N;
    std::string record;

    std::vector<float> record_id;
    float r;
    std::vector<float> mimoid_id;
    float m_id;
    std::vector<float> timestamp;
    float t;
    std::vector<float> length;
    float l;
    std::vector<float> width;
    float w;
    std::vector<float> height_max;
    float h_m;
    std::vector<float> height_avg;
    float h_a;
    std::vector<float> mounds;
    float mound;
    std::vector<std::string> color;
    float c;
    std::vector<float> age;
    float a;



    while(std::getline(std::cin, record))
    {
        std::istringstream iss(record);
		const char * c = record.c_str();
        if ( 3 == sscanf(c, "%f %f %f %f %f %f %f %f %c %f", r, m_id, t, l, w, h_m, h_a, mound, c, a))
        {
            record_id.push_back(r);
            mimoid_id.push_back(m_id);
            timestamp.push_back(t);
            length.push_back(l);
            width.push_back(w);
            height_max.push_back(h_m);
            height_avg.push_back(h_a);
            mounds.push_back(mound);
            color.push_back(c);
            age.push_back(a);

        }

    }

    for (unsigned i = 0; i < color.size(); i++)
    {
        for (unsigned j = 0; j < color[i].length(); j++)
        {
            if (color[i][j] != 'a' || color[i][j] != 'b' ||color[i][j] != 'c' ||color[i][j] != 'd' ||color[i][j] != 'e' ||color[i][j] != 'f' 
            ||color[i][j] != '0' ||color[i][j] != '1' ||color[i][j] != '2' ||color[i][j] != '3' ||color[i][j] != '4' ||color[i][j] != '5' ||color[i][j] != '6' 
            ||color[i][j] != '7' ||color[i][j] != '8' ||color[i][j] != '9')
            {
                
            }
        }

    }
}