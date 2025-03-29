/*Generates images of Mandelbrot set in complex space at different centers/resolutions in parallel to create a movie*/
#include "complex.h"
#include "mandelbrot.h"
#include "frameGenerator.h"
#include <omp.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>


void generateFrames(int max_procs, int n_frames, std::string region, bool save_frames, std::string output_dir, float zoom_factor, float standard_res){
    //Set the center of zoom based on region
    float r_center = 0.0f;
    float i_center = 0.0f;
    if(region == "Seahorse"){
        r_center = -0.743643887037151;
        i_center = 0.131825904205330;
    }
    else if(region == "Elephant Valley"){
        r_center = 0.282;
        i_center = 0.5307;       
    }
    else if(region == "Feigenbaum"){
        r_center = -1.401155;
        i_center = 0.0;             
    }
    else{
        printf("Region Undefined: {%s}, aborting", region.c_str());
        return;
    }

    //Frame generation constant params
    //float standard_res = 0.005f;    //Scaled with zoom
    float standard_width = 4.0f;     //Scaled with zoom
    float standard_height = 4.0f;    //Scaled with zoom
    int max_iter = 100;
    float thresh = 2.0f;
    //Set amount of zoom between frames, TODO: Make it depend on n_frames
    //float zoom_factor = 1.01;
    auto start_time = std::chrono::high_resolution_clock::now();
    //Generate frames in parallel
#pragma omp parallel for num_threads(max_procs)
    for (int frame = 0; frame < n_frames; frame++) {
        //Scale resolution
        float res = standard_res/std::pow(zoom_factor, frame);
        //Calculate width, height, and image borders
        float width = standard_width*(res/standard_res);
        float height = standard_height*(res/standard_res);
        float r_min = r_center - width/2.0f;
        float r_max = r_center + width/2.0f;
        float i_min = i_center - height/2.0f;
        float i_max = i_center + height/2.0f;

        //Create Mandelbrot instance
        Mandelbrot mandelbrot(max_iter, thresh, res, r_max, r_min, i_max, i_min);
        mandelbrot.generateImage();

        //Create filename and save image
        std::ostringstream filename;
        filename << output_dir << "frame_" << std::setw(4) << std::setfill('0') << frame << ".ppm";
        mandelbrot.saveImage(filename.str());

        //Print progress
        #pragma omp critical
        {
            int thread_idx = omp_get_thread_num();
            printf("Thread %3d: Generated Frame %4d of %4d at resolution %.10f\n", thread_idx, frame, n_frames, res);
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end_time - start_time).count();
    printf("Frame generation completed in %.10f seconds\n", duration);

    return;
}