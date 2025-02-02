#include "complex.h"
#include <iostream>

//Adding two complex numbers
Complex operator+(const Complex& a, const Complex& b){
    float real_sum = a.getReal() + b.getReal();
    float imag_sum = a.getImag() + b.getImag();
    
    return Complex(real_sum, imag_sum);
}

//Subtracting two complex numbers
Complex operator-(const Complex& a, const Complex& b){
    float real_sub = a.getReal() - b.getReal();
    float imag_sub = a.getImag() - b.getImag();

    return Complex(real_sub, imag_sub);
}

//Multiplying two complex numbers
Complex operator*(const Complex& a, const Complex& b){
    float real_mul = a.getReal()*b.getReal() - a.getImag()*b.getImag();
    float imag_mul = a.getReal()*b.getImag() + b.getReal()*a.getImag();

    return Complex(real_mul, imag_mul);
}  

//Mandlebrot function
Complex f_c(const Complex& z, const Complex& c){
    Complex f = z*z;
    return (f+c);
}

//Calculate vector magnitude
float magnitude(const Complex& a){
    float mag = sqrt(a.getReal()*a.getReal() + a.getImag()*a.getImag());

    return mag;
}

//Normalize vector into a unit vector
Complex unit(const Complex& a){
    Complex b(a.getReal()/magnitude(a), a.getImag()/magnitude(a));

    return b;
}

//Helper functions
void printCoord(const Complex& a){
    printf("%.6f + %.6fi\n", a.getReal(), a.getImag());
}
