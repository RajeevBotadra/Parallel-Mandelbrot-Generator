#include "complex.h"
#include "mandelbrot.h"
#include <omp.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>

/*Map an iteration to pixel value, higher iterations/convergence produce darker pixel; using Escape Time Convergence*/
Pixel Mandelbrot::mapPixel(int iter){
    if(this->max_iter == 0) return Pixel();
    // //Normalized iteration count
    double k = (double)(iter) / max_iter;
    k = pow(k, 0.5);
    double k_ = 1-k;
    //Escape Time Convergence mappaing
    int r = (int)(9*k_*k*k*k*255);
    int g = (int)(15*k_*k_*k*k*255);
    int b = (int)(8.5*k_*k_*k_*k*255);

    return Pixel(r, g, b);
}

/*Assign each point in grid a complex value*/
void Mandelbrot::makeGrid(){
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            float real = (float)j*this->resolution + r_min;
            float imag = (float)i*this->resolution + i_min;

            this->grid[i][j] = Complex(real, imag);
        }
    }

    return;
}

/*Evolves Mandelbrot set for a given starting value c*/
int Mandelbrot::generateSet(Complex c){
    Complex f = c;
    int iter = 0;

    while(iter < this->max_iter){
        if(magnitude(f) > this->threshold) break;
        iter++;
        f = f*f + c;
    }

    return iter;
}

/*Iterate through all points in grid to generate image*/
void Mandelbrot::generateImage(){
    makeGrid();
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            Complex c = this->grid[i][j];
            int iter = generateSet(c);
            Pixel p = mapPixel(iter);
            this->image[i][j] = p;
        }
    }

    return;
}

/*Save image as .ppm file*/
void Mandelbrot::saveImage(const std::string& filename){
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    int width = n_cols;
    int height = n_rows;

    //PPM file header
    file << "P3\n" << width << " " << height << "\n255\n"; 

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            Pixel p = this->image[i][j];
            file << p.r << " " << p.g << " " << p.b << " ";
            //file << this->grid[i][j].getReal() << " + i" << this->grid[i][j].getImag() << " ";
        }

        file << "\n";
    }

    file.close();
    return;
}

/*Override derived functions for parallel mandlebrot class*/
/*Assign each point in grid a complex value*/
void ParallelMandelbrot::makeGrid(){
#pragma omp parallel num_threads(this->max_procs)
{ 
    #pragma omp for collapse(2)
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            float real = (float)j*this->resolution + r_min;
            float imag = (float)i*this->resolution + i_min;

            this->grid[i][j] = Complex(real, imag);
        }
    }
}
    return;
}

/*Iterate through all points in grid to generate image and compute sets in parallel*/
void ParallelMandelbrot::generateImage(){
    makeGrid();
#pragma omp parallel num_threads(this->max_procs)
{ 
    //Use a dynamic scheduler since set generation will take varying amount of time 
    #pragma omp for schedule(dynamic, 16)
    
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            Complex c = this->grid[i][j];
            int iter = generateSet(c);
            Pixel p = mapPixel(iter);
            this->image[i][j] = p;
        }
    }
}

    return;
}
