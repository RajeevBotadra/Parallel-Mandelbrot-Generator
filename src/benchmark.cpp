#include <chrono>
#include <fstream>
#include <iostream>
#include "benchmark.h"
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