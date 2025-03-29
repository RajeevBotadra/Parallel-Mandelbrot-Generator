#ifndef FRAMEGENERATOR_H_
#define FRAMEGENERATOR_H_

#include "complex.h"
#include "mandelbrot.h"
#include <omp.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

/*Generates images of Mandelbrot set in complex space at different centers/resolutions in parallel to create a movie*/
void generateFrames(int max_procs, int n_frames, std::string region, bool save_frames, std::string output_dir, float zoom_factor, float standard_res);

#endif