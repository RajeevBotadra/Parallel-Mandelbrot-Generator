#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include <chrono>
#include <fstream>
#include <iostream>
#include <string.h>

class Benchmark{
/*Records compute time and experiment metadata and produces logfiles*/
private:
    //Start time
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    //End time
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    //Duration
    double duration;

    //Experiment metadata
    //Resolution of image
    float resolution;
    //Max number of iterations to test for divergence
    int max_iter;
    //Number of processors used 
    int n_procs;

    //Directory to log results
    std::string log_dir;
public:
    //Default params
    Benchmark(){
        this->resolution = 1.0f;
        this->max_iter = 255;
        this->n_procs = 1;
        this->log_dir = "./logs";
    }

    Benchmark(float resolution, int max_iter, int n_procs){
        this->resolution = resolution;
        this->max_iter = max_iter;
        this->n_procs = n_procs;  
        this->log_dir = "./logs";
    }

    //Specift logdir
    Benchmark(float resolution, int max_iter, int n_procs, std::string& log_dir){
        this->resolution = resolution;
        this->max_iter = max_iter;
        this->n_procs = n_procs;  
        this->log_dir = log_dir;
    }

    //Start clock
    void start();

    //Stop clock and return duration
    double stop();

    //Append results to logfile
    void log(std::string& file_name);
};


#endif