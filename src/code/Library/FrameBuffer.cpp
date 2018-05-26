/*
 * FrameBuffer.cpp
 *
 * Li-Yi Wei
 *
 */

#pragma warning (disable:4786)
#include <algorithm>
using namespace std;

#include "Exception.hpp"
#include "FrameBuffer.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

int FrameBuffer::ReadPGM(const string & file_name,
                         Array<P2> & pixels,
                         int & maximum_value)
{
    ifstream input(file_name.c_str());

    if(!input)
    {
        return 0;
    }
    else
    {
        // check magic number
        string magic;
        
        input >> magic;
        if(magic != "P2") return 0;
        
        // height, width, maximum
        int height, width;
        input >> width >> height >> maximum_value;
        if(input.bad()) return 0;

        // pixels
        vector<int> size_spec(2);
        size_spec[0] = height; size_spec[1] = width;
        pixels = Array<P2>(size_spec);

        vector<int> coord(2);
        P2 pixel;
        //for(int row = height-1; row >= 0; row--)
        for(int row = 0; row < height; row++)
        {
            for(int col = 0; col < width; col++)
            {
                coord[0] = row; coord[1] = col;
                input >> pixel;
          
                pixels.Put(coord, pixel);
                if(input.bad()) return 0;
            }
        }
    }

    return 1;
}

int FrameBuffer::WritePGM(const Array<P2> & pixels,
                          const int maximum_value,
                          const string & file_name)
{
    if(pixels.Dimension() != 2)
    {
        return 0;
    }
    
    ofstream output(file_name.c_str());

    if(!output)
    {
        return 0;
    }
    else
    {
        // magic number
        output << "P2" << endl;

        const int height = pixels.Size(0);
        if(height <= 0)
        {
            return 0;
        }

        const int width = pixels.Size(1);
        
        // header
        output << width << " " << height << " " << maximum_value << endl;

        // content
        vector<int> coord(2);
        P2 pixel;
        //for(int i = height-1; i >= 0; i--)
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
            {
                coord[0] = i; coord[1] = j;
                if(! pixels.Get(coord, pixel))
                {
                    return 0;
                }
                
                output << pixel << endl;
            }
        
        return 1;
    }
}

int FrameBuffer::ReadPPM(const string & file_name,
                         Array<P3> & pixels,
                         int & maximum_value)
{
    ifstream input(file_name.c_str());

    if(!input)
    {
        return 0;
    }
    else
    {
        // check magic number
        string magic;
        
        input >> magic;
        if(magic != "P3") return 0;
        
        // height, width, maximum
        int height, width;
        input >> width >> height >> maximum_value;
        if(input.bad()) return 0;

        // pixels
        vector<int> size_spec(2);
        size_spec[0] = height; size_spec[1] = width;
        pixels = Array<P3>(size_spec);

        vector<int> coord(2);
        P3 pixel;
        //for(int row = height-1; row >= 0; row--)
        for(int row = 0; row < height; row++)
        {
            for(int col = 0; col < width; col++)
            {
                coord[0] = row; coord[1] = col;
                input >> pixel.r;
                input >> pixel.g;
                input >> pixel.b;

                pixels.Put(coord, pixel);
                if(input.bad()) return 0;
            }
        }
    }

    return 1;
}

int FrameBuffer::WritePPM(const Array<P3> & pixels,
                          const int maximum_value,
                          const string & file_name)
{
    if(pixels.Dimension() != 2)
    {
        return 0;
    }
    
    ofstream output(file_name.c_str());

    if(!output)
    {
        return 0;
    }
    else
    {
        // magic number
        output << "P3" << endl;

        const int height = pixels.Size(0);
        if(height <= 0)
        {
            return 0;
        }

        const int width = pixels.Size(1);
        
        // header
        output << width << " " << height << " " << maximum_value << endl;

        // content
        vector<int> coord(2);
        P3 pixel;
        //for(int i = height-1; i >= 0; i--)
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
            {
                coord[0] = i; coord[1] = j;
                if(! pixels.Get(coord, pixel))
                {
                    return 0;
                }
                
                output << pixel.r << " " << pixel.g << " " <<  pixel.b << endl;
            }
        
        return 1;
    }
}

int FrameBuffer::ReadF2M(const string & file_name,
                         Array<F2> & pixels)
{
    ifstream input(file_name.c_str());

    if(!input)
    {
        return 0;
    }
    else
    {
        // check magic number
        string magic;
        
        input >> magic;
        if(magic != "F2") return 0;
        
        // dimension
        int dimension;
        input >> dimension;
        if(input.bad()) return 0;
        if(dimension <= 0) return 0;
        
        // sizes
        vector<int> sizes(dimension);
        int total_num_pixels = 1;
        {
            for(unsigned int i = 0; i < sizes.size(); i++)
            {
                input >> sizes[i];
                if(input.bad()) return 0;
                if(sizes[i] <= 0) return 0;

                total_num_pixels *= sizes[i];
            }
        }
        
        // pixels
        vector<F2> pixels_read(total_num_pixels);

        for(unsigned int i = 0; i < pixels_read.size(); i++)
        {
                input >> pixels_read[i];
                if(input.bad()) return 0;
        }
        
        pixels = Array<F2>(sizes);
        if(! pixels.Put(pixels_read))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

int FrameBuffer::WriteF2M(const Array<F2> & pixels,
                          const string & file_name)
{
    ofstream output(file_name.c_str());

    if(!output)
    {
        return 0;
    }
    else
    {
        // magic number
        output << "F2" << endl;

        // header
        output << pixels.Dimension() << endl;

        {
            for(int i = 0; i < pixels.Dimension(); i++)
            {
                output << pixels.Size(i) << " ";
            }
            output << endl;
        }
        
        // content
        vector<F2> pixels_write;
        if(! pixels.Get(pixels_write))
        {
            return 0;
        }
        
        for(unsigned int i = 0; i < pixels_write.size(); i++)
        {
            output << pixels_write[i] << " " << endl;
        }
        
        return 1;
    }
}

int FrameBuffer::ReadPFM(const string & file_name,
                         Array<PF> & pixels)
{
    ifstream input(file_name.c_str());

    if(!input)
    {
        return 0;
    }
    else
    {
        // check magic number
        string magic;
        
        input >> magic;
        if(magic != "PF") return 0;
        
        // dimension
        int dimension;
        input >> dimension;
        if(input.bad()) return 0;
        if(dimension <= 0) return 0;
        
        // sizes
        vector<int> sizes(dimension);
        int total_num_pixels = 1;
        {
            for(unsigned int i = 0; i < sizes.size(); i++)
            {
                input >> sizes[i];
                if(input.bad()) return 0;
                if(sizes[i] <= 0) return 0;

                total_num_pixels *= sizes[i];
            }
        }
        
        // pixels
        vector<PF> pixels_read(total_num_pixels);

        for(unsigned int i = 0; i < pixels_read.size(); i++)
        {
                input >> pixels_read[i].r;
                input >> pixels_read[i].g;
                input >> pixels_read[i].b;
                if(input.bad()) return 0;
        }
        
        pixels = Array<PF>(sizes);
        if(! pixels.Put(pixels_read))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

int FrameBuffer::WritePFM(const Array<PF> & pixels,
                          const string & file_name)
{
    ofstream output(file_name.c_str());

    if(!output)
    {
        return 0;
    }
    else
    {
        // magic number
        output << "PF" << endl;

        // header
        output << pixels.Dimension() << endl;

        {
            for(int i = 0; i < pixels.Dimension(); i++)
            {
                output << pixels.Size(i) << " ";
            }
            output << endl;
        }
        
        // content
        vector<PF> pixels_write;
        if(! pixels.Get(pixels_write))
        {
            return 0;
        }
        
        for(unsigned int i = 0; i < pixels_write.size(); i++)
        {
            output << pixels_write[i].r << " " << pixels_write[i].g << " " <<  pixels_write[i].b << endl;
        }
        
        return 1;
    }
}
