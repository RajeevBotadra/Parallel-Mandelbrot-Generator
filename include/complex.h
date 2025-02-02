#ifndef COMPLEX_H_
#define COMPLEX_H_
/*Defines a class to represent complex numbers and operate on them*/

#include <iostream>
#include <math.h>

class Complex{
private:
    //Real and imaginary components of numbers
    float real;
    float imag;

public:
    //Default Constructor
    Complex(){
        this->real = 0.0f;
        this->imag = 0.0f;
    }

    //Constructor
    Complex(float real, float imag){
        this->real = real;
        this->imag = imag;
    }

    float getReal() const{
        return this->real;
    }

    float getImag() const{
        return this->imag;
    }
};

//Adding two complex numbers
Complex operator+(const Complex& a, const Complex& b);

//Subtracting two complex numbers
Complex operator-(const Complex& a, const Complex& b);

//Multiplying two complex numbers
Complex operator*(const Complex& a, const Complex& b);     

//Mandlebrot function
Complex f_c(const Complex& z, const Complex& c);

/*-------------------Helper Functions-------------------*/
//Calculate vector magnitude
float magnitude(const Complex& a);

//Normalize vector into a unit vector
Complex unit(const Complex& a);

//Print Coordinate
void printCoord(const Complex& a);

#endif