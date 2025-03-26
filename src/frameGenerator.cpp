/*Generates images of Mandelbrot set in complex space at different centers/resolutions in parallel to create a movie*/
#include "complex.h"
#include "mandelbrot.h"
#include <omp.h>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

/*Arguments:
*   argv[1] -- (int) number of processors to use in generation
*   argv[2] -- (int) number of frames to generate
*   argv[3] -- (char*) region to generate ("Seahorse", "Elephant Valley", "Feigenbaum")
*   argv[4] -- (bool) save files or not
*   argv[5] -- (char*) dir to save frames
*/
int main(int argc, char** argv){
    //Parse args
    int max_procs = 1;
    int n_frames = 10;
    std::string region = "Feigenbaum";
    bool save_frames = true;
    std::string output_dir = "../figures/frames/";
    if(argc == 6){
        max_procs = std::stoi(argv[1]);
        n_frames = std::stoi(argv[2]);
        output_dir = std::string(argv[3]);
        save_frames = std::stoi(argv[4]) == 1;
        output_dir = std::string(argv[5]);
        printf("Using %d processors to generate %d frames of %s\n", max_procs, n_frames, region.c_str());
        if(save_frames) printf("Saving generated frames to %s\n", output_dir.c_str());
        else printf("Not saving generated frames\n");
    }
    else{
        printf("Insufficient arguments, using %d processors to generate %d frames of %s\n", max_procs, n_frames, region.c_str());
        printf("Saving generated frames to %s\n", output_dir.c_str());
    }

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

    //Frame generation constant params
    float standard_res = 0.005f;    //Scaled with zoom
    float standard_width = 4.0f;     //Scaled with zoom
    float standard_height = 4.0f;    //Scaled with zoom
    int max_iter = 100;
    float thresh = 2.0f;
    //Set amount of zoom between frames, TODO: Make it depend on n_frames
    float zoom_factor = 1.01;
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
        ParallelMandelbrot mandelbrot(max_procs, max_iter, thresh, res, r_max, r_min, i_max, i_min);
        mandelbrot.generateImage();

        //Create filename and save image
        std::ostringstream filename;
        filename << output_dir << "frame_" << std::setw(4) << std::setfill('0') << frame << ".ppm";
        mandelbrot.saveImage(filename.str());

        //Print progress
        #pragma omp critical
        {
            printf("Generated Frame %d of %d at resolution %.7f\n", frame, n_frames, res);
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end_time - start_time).count();
    printf("Frame generation completed in %.10f seconds\n", duration);

    return 0;
}
