#include "complex.h"
#include "mandelbrot.h"
#include <iostream>
#include "omp.h"

int main(){
    //Test Complex
    Complex x(5.0, 7.0);
    Complex y(-4.5, 9.2);

    printCoord(x);
    printCoord(y);
    printCoord(x+y);
    printCoord(x-y);
    printCoord(x*y);

    //Test Mandelbrot
    //Maximum number of iterations before declaring convergence
    int max_iter = 1000;
    //Threshold for divergence
    float thresh = 2.0f;
    //The distance between two sample points (i.e. delta_x, delta_y)
    float res = 0.005f;
    //Range of the space we are sampling in
    float r_max = 4.0f;
    float r_min = -4.0f;
    float i_max = 4.0f;
    float i_min = -4.0f;

    //High res example
    Mandelbrot a(max_iter, thresh, res, r_max, r_min, i_max, i_min);
    //Simple example
    //Mandelbrot a;

    std::string filename = "../figures/image.ppm";
    a.generateImage();
    a.saveImage(filename);
    return 0;
}