'''
Converts generated Mandelbrot frames into video
'''
import os
import argparse
import subprocess

def createVideo(frames_dir, output_dir, fps):
    '''
    Takes .ppm images as frames to video via ffmpeg
    Args:
        frames_dir (str) - Directory where frames are located
        output_dir (str) - Directory to store video
        fps (int) - Determines intraframe time (1/fps) 
    Returns: None
    '''
    #Check dir exists
    if not os.path.exists(frames_dir):
        print(f'Frames directory "{frames_dir}" does not exist')
        return
    
    frame_pattern = os.path.join(frames_dir, "frame_%04d.ppm")
    if not any(fname.endswith(".ppm") for fname in os.listdir(frames_dir)):
        print(f"Error: No .ppm frames found in '{frames_dir}'.")
        return
    
    #Create an ffmpeg command
    cmd = [
        "ffmpeg",
        "-framerate", str(fps),
        "-i", frame_pattern,
        "-c:v", "libx264",
        "-pix_fmt", "yuv420p",
        "-crf", "23",  # Quality setting (lower is better, 18-28 recommended)
        output_dir
    ]

    #Run the ffmpeg command
    try:
        subprocess.run(cmd, check=True)
        print(f"Video saved as {output_dir}")
    except subprocess.CalledProcessError as e:
        print(f"Error: ffmpeg failed with error {e}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Convert Mandelbrot frames to a video.")
    parser.add_argument("input_dir", type=str, help="Directory containing Mandelbrot frames")
    parser.add_argument("output_file", type=str, help="Output video file name (e.g., mandelbrot.mp4)")
    parser.add_argument("--fps", type=int, default=30, help="Frames per second for the video (default: 30)")

    args = parser.parse_args()
    createVideo(args.input_dir, args.output_file, args.fps)    
