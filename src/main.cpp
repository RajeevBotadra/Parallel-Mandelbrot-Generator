/*Generates images of Mandelbrot set in complex space at different centers/resolutions in parallel to create a movie*/
#include "complex.h"
#include "frameGenerator.h"
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
*   argv[6] -- (float) zoom factor between frames, 1.025 is a 2.5% zoom between frames
*   argv[7] -- (float) standard resolution of a frame with no zoom (will be scaled with dimensions)
*/
int main(int argc, char** argv){
    //Parse args
    int max_procs = 1;
    int n_frames = 10;
    std::string region = "Feigenbaum";
    bool save_frames = true;
    std::string output_dir = "../figures/frames/";
    float zoom_factor = 1.025f;
    float standard_res = 0.001f;    //Scaled with zoom

    if(argc == 8){
        max_procs = std::stoi(argv[1]);
        n_frames = std::stoi(argv[2]);
        region = std::string(argv[3]);
        save_frames = std::stoi(argv[4]) == 1;
        output_dir = std::string(argv[5]);
        zoom_factor = std::stof(argv[6]);
        standard_res = std::stof(argv[7]);
        printf("Using %d processors to generate %d frames of %s at a %.6f standard resolution and %.4f zoom factor\n", 
        max_procs, n_frames, region.c_str(), standard_res, zoom_factor);
        if(save_frames) printf("Saving generated frames to %s\n", output_dir.c_str());
        else printf("Not saving generated frames\n");
    }
    else{
        printf("Insufficient arguments, using %d processors to generate %d frames of %s at a %.6f standard resolution and %.4f zoom factor\n"
        , max_procs, n_frames, region.c_str(), standard_res, zoom_factor);
        printf("Saving generated frames to %s\n", output_dir.c_str());
    }

    //Generate frames
    generateFrames(max_procs, n_frames, region, save_frames, output_dir, zoom_factor, standard_res);

    return 0;
}
