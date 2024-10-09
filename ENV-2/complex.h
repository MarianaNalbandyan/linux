#ifndef COMPLEX_H
#define COMPLEX_H
#include <iostream>
#include <cmath>

class Complex 
{
public:
    Complex(double r = 0.0, double i = 0.0);
    
    double getReal() const;
    double getImag() const:
    
    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(double scalar) const;
    double abs() const;
    
    friend std::ostream& operator<<(std::ostream& os, const Complex& g);
    
private:
    double r;
    double i;
};

#endif
