#include "complex.h"
    Complex::Complex(double r, double i) : r(r), i(i) {}
    
    Complex Complex::operator + (const Complex& other) const 
    {
       return Complex(r + other.r, i + other.i);
    }
    
    Complex Complex::operator-(const Complex& other) const
    {
       return Complex(r - other.r, i - other.i);
    }
    
    Complex Complex::operator*(double scalar) const
    {
       return Complex(r * scalar, i * scalar);
    }
    
    double Complex::abs() const
    {
       return sqrt(r * r + i * i);
    }
    
    std::ostream& operator<<(std::ostream& os, const Complex& g)
    {
       os << "(" << g.r << "+" << g.i << "i)";
       return os;
    }
