#include <chrono>
#include <vector>
#include <map>
#include <omp.h>
#include <fstream>
#include <iostream>
#include "benchmark.h"
#include "mandelbrot.h"
#include <string.h>

//Start clock
void Benchmark::start(){
    this->start_time = std::chrono::high_resolution_clock::now();
    return;
}

//Stop clock and return duation
double Benchmark::stop(){
    this->end_time = std::chrono::high_resolution_clock::now();
    this->duration = std::chrono::duration<double>(end_time - start_time).count();

    return this->duration;
}

//Append results to logfile
void Benchmark::log(std::string& file_name){
    std::string file_path = this->log_dir + "/" + file_name;
    //Open file in appending mode
    std::ofstream log_file(file_path, std::ios::app);
    if(log_file){
        log_file << this->n_procs << ", " << this->resolution << ", " << this->max_iter << ", " << this->duration << "\n";
    }
    else{
        std::cerr << "Could not open logfile: " << file_path << "\n";
    }

    log_file.close();
    return;
}

void runBenchmark(){
    /*Setup Experiments for fine grained benchmark*/
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
    std::map<std::pair<int, float>, double> results;

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
    return;
}