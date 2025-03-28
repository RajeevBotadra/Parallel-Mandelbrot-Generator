#ifndef MANDELBROT_H_
#define MANDELBROT_H_

#include "complex.h"
#include <vector>
#include <math.h>
#include <omp.h>

/*Construct a point in a 2D grid*/
struct Pixel{
    int r;
    int g;
    int b;

    //Default is black pixel
    Pixel() : r(0), g(0), b(0) {}
    Pixel(int r, int g, int b) : r(r), g(g), b(b) {}
};

/*Define the space in which the set is generated and the resolution with with the space is sampled*/
class Mandelbrot{
protected:
    //Maximum number of iterations before declaring convergence
    int max_iter;
    //Threshold for divergence
    float threshold;
    //The distance between two sample points (i.e. delta_x, delta_y)
    float resolution;
    //Range of the space we are sampling in
    float r_max, r_min, i_max, i_min;

    //Declare space in which set exists as a 2D array
    int n_rows, n_cols;
    std::vector<std::vector<Complex>> grid;

    //Declare space in solution exists (each entry is a pixel value) determined by number of iterations
    std::vector<std::vector<Pixel>> image;
public:
    //Default constructor, sets a 20x20 square grid to sample from. Max_iter=255 and a threshold=2.
    Mandelbrot(){
        this->max_iter = 255;
        this->threshold = 2.0f;
        this->resolution = 1.0f;
        this->r_max = 10.0f;
        this->r_min = -10.0f;
        this->i_max = 10.0f;
        this->i_min = -10.0f;

        //Now initialize the grid and image
        this->n_rows = ceil((r_max - r_min)/resolution);
        this->n_cols = ceil((i_max - i_min)/resolution);
        this->grid = std::vector<std::vector<Complex>>(n_rows, std::vector<Complex>(n_cols, Complex()));
        this->image = std::vector<std::vector<Pixel>>(n_rows, std::vector<Pixel>(n_cols, Pixel()));

    }

    Mandelbrot(int max_iter, float thresh, float res, float r_max, float r_min, float i_max, float i_min){
        this->max_iter = max_iter;
        this->threshold = thresh;
        this->resolution = res;
        this->r_max = r_max;
        this->r_min = r_min;
        this->i_max = i_max;
        this->i_min = i_min;

        //Now initialize the grid and image
        this->n_rows = ceil((r_max - r_min)/resolution);
        this->n_cols = ceil((i_max - i_min)/resolution);
        this->grid = std::vector<std::vector<Complex>>(n_rows, std::vector<Complex>(n_cols, Complex()));
        this->image = std::vector<std::vector<Pixel>>(n_rows, std::vector<Pixel>(n_cols, Pixel()));
    }

    //Map an iteration to pixel value, higher iterations/convergence produce darker pixel; using Escape Time Convergence
    Pixel mapPixel(int iter);

    //Builds the grid of complex values
    void makeGrid();

    //Evolves Mandelbrot set for a given starting value c
    int generateSet(Complex c);

    //Generates set, serial computation
    void generateImage();

    //Save image as a .ppm file
    void saveImage(const std::string& filename);

};


/*Parallelize generation*/
class ParallelMandelbrot : public Mandelbrot{
protected:
    //Num cpus to use, actual to be allocated by os
    int max_procs;
public:
    //Default constructor, sets a 20x20 square grid to sample from. Max_iter=255 and a threshold=2.
    ParallelMandelbrot() : Mandelbrot(){
        this->max_procs = 32;
        omp_set_num_threads(this->max_procs);
    }

    ParallelMandelbrot(int max_procs, int max_iter, float thresh, float res, float r_max, float r_min, float i_max, float i_min)
    : Mandelbrot(max_iter, thresh, res, r_max, r_min, i_max, i_min){
        this->max_procs = max_procs;
        omp_set_num_threads(this->max_procs);  
    }

    //Builds the grid of complex values in parallel
    void makeGrid();

    //Generates image by evolving sets in parallel
    void generateImage();

};

#endif