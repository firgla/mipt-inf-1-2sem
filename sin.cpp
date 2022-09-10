#include <iostream>
#include <cmath>

double fmod(double const x, double const y)
{
    return x - y * int(x / y);
}

double fsin(double x)
{
    x = fmod(x, 2 * M_PI);
    double i = 1.;
    double sum = 0.;
    double summand_i = x;

    while (sum != sum + summand_i)
    {
        sum += summand_i;
        summand_i *= -x * x / (2. * i) / (2. * i + 1.);
        i += 1.;
    }
    return sum;
}

#include <iomanip>
int main()
{
    double x;
    std::cin >> x;
    std::cout<< std::setprecision(16);
    std::cout << fsin(x) << std::endl;
    std::cout << std::sin(x) << std::endl;  
}