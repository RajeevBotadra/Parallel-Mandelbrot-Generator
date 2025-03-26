#include "complex.h"
#include "mandelbrot.h"
#include <omp.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>

/*Map an iteration to pixel value, higher iterations/convergence produce darker pixel*/
int Mandelbrot::mapPixel(int iter){
    if(this->max_iter == 0) return 0;
    int pixel = floor(255*(max_iter - iter)/max_iter);

    return pixel;
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

    return mapPixel(iter);
}

/*Iterate through all points in grid to generate image*/
void Mandelbrot::generateImage(){
    makeGrid();
    for(int i = 0; i < n_rows; i++){
        for(int j = 0; j < n_cols; j++){
            Complex c = this->grid[i][j];
            int pixel = generateSet(c);
            this->image[i][j] = pixel;
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
            int pixel = this->image[i][j];
            file << pixel << " " << pixel << " " << pixel << " ";
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
            int pixel = generateSet(c);
            this->image[i][j] = pixel;
        }
    }
}

    return;
}
