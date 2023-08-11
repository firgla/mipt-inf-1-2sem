

/*
1 задача
class Complex
{
    double x, y;
public:
    Complex(double re = 0., double im = 0.);
    double real() const;
    double imag() const;
    Complex &operator+=(Complex const &);
    Complex &operator-=(Complex const &);
    Complex &operator*=(Complex const &);
    Complex &operator/=(Complex const &);
};
double norm(Complex const &);
double  arg(Complex const &);
Complex operator+(Complex const &, Complex const &);
Complex operator-(Complex const &, Complex const &);
Complex operator*(Complex const &, Complex const &);
Complex operator/(Complex const &, Complex const &);


Complex(double re, double im)
{
    x = re;
    y = im;
}

double Complex::real() const
{
    return x;
}
double Complex::imag() const
{
    return y;
}


double norm(Complex const &a)
{
    return a.real() * a.real() + a.imag() * a.imag();
}

double  arg(Complex const &a)
{
    return atan2(a.imag() , a.real());
}

Complex operator+(Complex const &a, Complex const &b)
{
    return Complex(a.real() + b.real(),a.imag() + b.imag());
}

Complex operator-(Complex const &a, Complex const &b)
{
    return Complex(a.real() - b.real(),a.imag() - b.imag());
}

Complex operator*(Complex const &a, Complex const &b)
{
    return Complex(a.real()*b.real() - a.imag() * b.imag(), a.real()*b.imag() + b.real()*a.imag());
}

Complex operator/(Complex const &a, Complex const &b)
{
    return Complex((a.real() * b.real() + a.imag() * b.imag()) / (b.real() * b.real() + b.imag() * b.imag()), (a.imag() * b.real() - a.real() * b.imag()) / (b.real() * b.real() + b.imag() * b.imag()));
}

Complex &Complex::operator+=(Complex const &a)
{
    x += a.real();
    y += a.imag();

    return *this;
}

Complex &Complex::operator-=(Complex const &a)
{
    x -= a.real();
    y -= a.imag();

    return *this;
}

Complex &Complex::operator*=(Complex const &a)
{
    *this = {x * a.real() - y * a.imag(), x * a.imag() + y * a.real()};
    return *this;
}

Complex &Complex::operator/=(Complex const &a)
{
    *this = {(x * a.real() + y * a.imag()) / (a.real() * a.real() + a.imag() * a.imag()), (y * a.real() - x * a.imag()) / (a.real() * a.real() + a.imag() * a.imag())};
    return *this;

}

*/

