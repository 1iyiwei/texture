import os
import string
import shutil
import sys
import glob
import argparse
import fileinput
import shutil

def ppm_file_path(input_file_path):
    dot_position = input_file_path.rfind(".");
    if(dot_position >= 0):
        answer = input_file_path[:(dot_position + 1)] + "ppm";
    else:
        answer = "";
    return answer;

def remove_file(victim_file_path):
    if os.path.isfile(victim_file_path):
        os.remove(victim_file_path);

def main():
    parser = argparse.ArgumentParser(description='tile');
    parser.add_argument("input_image")
    parser.add_argument("output_image")
    parser.add_argument("-b", "--input_boundary", default = "none", help='input boundary condition (none or toroidal)');    
    parser.add_argument("--exe_folder", default = os.path.join(".", "bin", "Release"), help="folder for the executable file");
    args = parser.parse_args()

    if(not shutil.which("magick")):
        raise ValueError('need magick on your path; see https://www.imagemagick.org/');
    magick = "magick";

    input_image = args.input_image;
    output_image = args.output_image;
    input_image_ppm = ppm_file_path(input_image);
    output_image_ppm = ppm_file_path(output_image);

    input_boundary = args.input_boundary;
    bin_dir = args.exe_folder;

    pass_separator = '+'
    level_separator = '*'
    pyramid_separator = '-'
    param_group_separator = '.'

    # coherencex.y: x spatial coherence neighbors and y random neighbors for extra consideration
    synthesis_spec = "seam_random"+param_group_separator.join(['1', '1']);
    sequence_spec = "randomshuffle1";
    neighborhood_spec = "square3";

    clean_up = True;

    # input conversion
    command = magick + " " + input_image + " -compress none " + input_image_ppm;
    os.system(command);

    # tile
    command = os.path.join(bin_dir, "Tile") + " " + input_image_ppm + " " + input_boundary + " " + synthesis_spec + " " + sequence_spec + " " + neighborhood_spec + " " + output_image_ppm;
    os.system(command);

    # output conversion
    command = magick + " " + output_image_ppm + " " + output_image;
    os.system(command);

    # cleanup
    if(clean_up):
        remove_file(input_image_ppm);
        remove_file(output_image_ppm);

if __name__ == "__main__":
    main();
