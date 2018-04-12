import os
import string
import shutil
import sys
import glob
import fileinput
import shutil
from texture import *

if False:
    input_file_name = "../data/161.jpg"
    num_levels = 6

    level_names = pyramid(input_file_name, num_levels)
    
    print(level_names)

if True:
    # parameters
    pass_separator = '+'
    level_separator = '*'
    pyramid_separator = '-'
    param_group_separator = '.'

    input_dir = "../data/"
    output_dir = "../data/"
    bin_dir = os.path.join("bin", "Release");

    input_file_base_name = "161"

    # the boundary conditions,
    # toroidal for tileable images
    # usually the input is not toroidal, so specify "none"
    input_boundary = "none"
    output_boundary = "toroidal"

    # synthesis methods for each pass
    # random for randomly copy from the input
    # coherence is the patch match coherence propagation
    # coherencex.y: x spatial coherence neighbors and y random neighbors for extra consideration
    synthesis_spec = pass_separator.join(["coherence"+param_group_separator.join(['0', '1']), "coherence"+param_group_separator.join([level_separator.join(['1', '0']), '1']), "coherence"+param_group_separator.join([level_separator.join(['1', '0']), '1'])])
    # synthesis_spec = pass_separator.join(["random", "coherence1.1", "coherence1.1"])

    # passes for each output image or pyramid level
    # scanline is the traditional scanline order
    # randomshuffle visits each pixel once without replacement, which can help break scanline regularity/bias
    sequence_spec = pass_separator.join(["scanline1", "scanline3", "randomshuffle3"])
    
    # number of pyramid levels for multi-resolution synthesis
    # to specify the neighborhoods, as shown next below
    num_levels = 4

    # neighborhood sizes and shapes, can be multi-resolution
    if False:
        neighborhood_spec = pyramid_separator.join([level_separator.join(["square3", "square2"])]*num_levels)
    else:
        neighborhood_spec = pyramid_separator.join([pass_separator.join(["square0", level_separator.join(["square3", "square2"]), "square2"])]*num_levels)

    # the size, height x width, of the output
    output_size = "119x131"
    output_file_base_name = input_file_base_name + "_" + output_size

    num_levels = len(neighborhood_spec.split(pyramid_separator));

    input_file_name = os.path.join(input_dir, input_file_base_name + ".jpg") 
    input_temp_name = os.path.join(input_dir, input_file_base_name + ".ppm")
    output_temp_name = os.path.join(output_dir, output_file_base_name + ".ppm")
    output_file_name = os.path.join(output_dir, output_file_base_name + ".png")

    # magick
    if(not shutil.which("magick")):
        raise ValueError('need magick on your path; see https://www.imagemagick.org/');
    magick = "magick";

    # input
    convert_command = magick + " -compress none " + input_file_name + " " + input_temp_name
    os.system(convert_command)

    # pyramid
    input_level_names = pyramid(input_temp_name, num_levels)
    input_level_name_join = pyramid_separator.join(input_level_names)

    # synthesis
    synth_command = os.path.join(bin_dir, "Synth") + " " + input_level_name_join + " " + input_boundary + " " + output_boundary + " " + synthesis_spec + " " + sequence_spec + " " + neighborhood_spec + " " + output_size + " " + output_temp_name
    os.system(synth_command)

    convert_command = magick + " " + output_temp_name + " " + output_file_name
    os.system(convert_command)

    # cleanup
    os.remove(input_temp_name)
    os.remove(output_temp_name)
    for input_level_name in input_level_names:
        os.remove(input_level_name)
