import os
import subprocess
import sys
import math
import string
import shutil
import json
import re

# build pyramid from an input image
# return an array of level file names
def pyramid(input_file_path, num_levels):

    filter = 'Lanczos';
    blur = .9891028367558475;

    # names
    [file_path, input_file_name] = os.path.split(input_file_path)

    name_parts = input_file_name.split('.');
    
    if(len(name_parts) != 2):
        raise ValueError('illegal input name:' + input_file_name);

    base_name = name_parts[0];
    type_name = name_parts[1];

    level_file_names = [];
    for level in range(num_levels):
        level_file_name = file_path;
        if(file_path != ''):
            level_file_name = level_file_name + '/';
        level_file_name = level_file_name + base_name + '_' + str(level) + '.' + type_name;
        level_file_names.append(level_file_name);

        # compute
        resize = 100.0/pow(2, level);
        downsample_command = 'convert ' + input_file_path + ' -colorspace RGB -filter ' + filter + ' -define filter:blur=' + str(blur) + ' -resize ' + str(resize) + '%' + ' -compress none ' + ' -colorspace RGB ' + level_file_name;
        os.system(downsample_command);

    # done
    return level_file_names;

