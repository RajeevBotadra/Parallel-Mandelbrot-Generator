#include "complex.h"
#include "mandelbrot.h"
#include "benchmark.h"
#include <iostream>
#include <map>
#include "omp.h"

void testComplex(){
    //Test Complex
    Complex x(5.0, 7.0);
    Complex y(-4.5, 9.2);

    printCoord(x);
    printCoord(y);
    printCoord(x+y);
    printCoord(x-y);
    printCoord(x*y);

    return;
}

/*Benchmark singlethreaded vs multithreaded generation*/
int main2(){
    /*Setup Experiments*/
    //Variables
    printf("Max Number of threads: %d\n", omp_get_max_threads());
    std::vector<int> thread_counts;
    std::vector<float> resolutions = {0.001, 0.01, 0.1, 1.0};
    int max_procs = 32;
    for(int i = 1; i <= max_procs; i++){
        thread_counts.push_back(i);
    }

    //Experiment constants
    //Maximum number of iterations before declaring convergence
    int max_iter = 1000;
    //Threshold for divergence
    float thresh = 2.0f;
    //The distance between two sample points (i.e. delta_x, delta_y)
    float res = 0.0005f;
    //Range of the space we are sampling in
    float r_max = 4.0f;
    float r_min = -4.0f;
    float i_max = 4.0f;
    float i_min = -4.0f;
    std::string log_name = "test.txt";
    std::string log_dir = "../logs";

    //Map to store results as [n_threds, resolution]:runtime map
    // std::map<std::pair<int, float>, double> results;

    //Benchmark
    Benchmark test(res, max_iter, max_procs, log_dir);
    test.start();

    //High res example
    ParallelMandelbrot a(max_procs, max_iter, thresh, res, r_max, r_min, i_max, i_min);
    //Simple example
    //Mandelbrot a;
    double dur = test.stop();
    printf("%.5f", dur);
    test.log(log_name);

    // std::string filename = "../figures/image.ppm";
    // a.generateImage();
    // a.saveImage(filename);
    return 0;
}