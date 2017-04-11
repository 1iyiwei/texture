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
#include "Random.hpp"

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

int FrameBuffer::ReadF2MB(const string & file_name,
                          Array<F2> & pixels)
{
#if 1
    FILE *fp = fopen(file_name.c_str(), "rb");

    if(! fp)
    {
        return 0;
    }
    else
    {
        char line[256];
        int w, h;
        float* buffer;
        fgets(line, 256, fp);   // line should be equal to string 'Pf'
        fgets(line, 256, fp);   // second line, contains width and height
        sscanf(line, "%d%d", &w, &h);
        fgets(line, 256, fp);   // third line is scale, ignored

        buffer = new float[w*h];
        fread(buffer, w*h, sizeof(float), fp);
        fclose(fp);

        // dimension
        const int dimension = 2;
        
        // sizes
        vector<int> sizes(dimension);
        sizes[0] = h; sizes[1] = w;
        int total_num_pixels = 1;
        {
            for(unsigned int i = 0; i < sizes.size(); i++)
            {
                if(sizes[i] <= 0) return 0;
            
                total_num_pixels *= sizes[i];
            }
        }
        
        // pixels
        vector<F2> pixels_read(total_num_pixels);

        for(unsigned int i = 0; i < pixels_read.size(); i++)
        {
            pixels_read[i] = buffer[i];
        }
        
        delete buffer;

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
#else
    ifstream input(file_name.c_str(), ios::binary);

    if(!input)
    {
        return 0;
    }
    else
    {
        // check magic number
        string magic;
        
        input >> magic;
        if(magic != "Pf") return 0;
        
        // dimension
        const int dimension = 2;
        
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
        
        // scale
        float scale;
        input >> scale;

        // pixels
        vector<F2> pixels_read(total_num_pixels);
        float temp = 0;
        unsigned char * p_temp = reinterpret_cast<unsigned char *>(&temp);

        for(unsigned int i = 0; i < pixels_read.size(); i++)
        {
            for(int j = 0; j < sizeof(temp)/sizeof(unsigned char); j++)
            {
                if(scale < 0)
                {
                    input >> p_temp[j];
                }
                else
                {
                    input >> p_temp[sizeof(temp)/sizeof(unsigned char)-j-1];
                }
            }

            pixels_read[i] = temp;
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
#endif
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

int FrameBuffer::WriteSVG(const vector<L2F> & samples, const vector<P3> & palette, const vector<float> & region, const float radius, const int boundary, const int color_scheme, const string & file_name)
{
    ofstream output(file_name.c_str());

    if(region.size() != 4)
    {
        return 0;
    }
    
    if((color_scheme < 0) || (color_scheme > 1))
    {
        return 0;
    }
    
    if(!output)
    {
        return 0;
    }
    else
    {
        // opening
        output << "<svg width=" << '"' << region[1] << '"' << " height=" << '"' << region[3] << '"';

        output << " viewBox=" << '"' << region[0] << " " << region[2] << " " << region[1] << " " << region[3] << '"';

        output << ">" << endl;

#if 0
        // boundar size meaning different from fig, usually too thick
        // boundary, if necessary
        if(boundary/* || (!boundary && (color_scheme == 1))*/)
        {
            output << "<rect" << " x=" << '"' << region[0] << '"' << " y=" << '"' << region[2] << '"' << " width=" << '"' << region[1] << '"' << " height=" << '"' << region[3] << '"' << " style=" << '"' << "fill:none;stroke:black;stroke-width:" << boundary << '"' << "/>" << endl;
        }
#endif

        // background, if necessary
        if(color_scheme)
        {
            output << "<rect" << " x=" << '"' << region[0] << '"' << " y=" << '"' << region[2] << '"' << " width=" << '"' << region[1] << '"' << " height=" << '"' << region[3] << '"' << " style=" << '"' << "fill:black" << '"' << "/>" << endl;
        }

        // samples
        for(unsigned int i = 0; i < samples.size(); i++)
        {
            const FrameBuffer::L2F & sample = samples[i];
            if((sample.color_index < 0) || (sample.color_index >= palette.size()))
            {
                return 0;
            }

            const FrameBuffer::P3 color = palette[sample.color_index];

            output << "<circle cx=" << '"' << sample.x << '"' << " cy=" << '"' << sample.y << '"';
            output << " r=" << '"' << radius << '"';
            output << " style=" << '"' << "fill:rgb(" << color.r << "," << color.g << "," << color.b << ");" << '"';
            output << "/>" << endl;
        }

        // closing
        output << "</svg>" << endl;

        return 1;
    }
}

int FrameBuffer::WriteFIG(const vector<L2F> & samples, const vector<P3> & palette, const vector<float> & region, const float radius, const int boundary, const int color_scheme, const string & file_name)
{
    vector<Tile> tiles;
    vector<Circle> circles;
    return WriteFIG(samples, tiles, circles, palette, region, radius, boundary, boundary, color_scheme, file_name);
}

int FrameBuffer::WriteFIG(const vector<Tile> & tiles, const vector<P3> & palette, const vector<float> & region, const int tile_boundary, const int region_boundary, const int color_scheme, const string & file_name)
{
    vector<L2F> samples;
    vector<Circle> circles;
    return WriteFIG(samples, tiles, circles, palette, region, 0, tile_boundary, region_boundary, color_scheme, file_name);
}

int FrameBuffer::WriteFIG(const vector<Circle> & circles, const vector<P3> & palette, const vector<float> & region, const int tile_boundary, const int region_boundary, const int color_scheme, const string & file_name)
{
    vector<L2F> samples;
    vector<Tile> tiles;
    return WriteFIG(samples, tiles, circles, palette, region, 0, tile_boundary, region_boundary, color_scheme, file_name);
}

int FrameBuffer::WriteFIG(const vector<L2F> & samples, const vector<Tile> & tiles, const vector<Circle> & circles, const vector<P3> & palette, const vector<float> & region, const float radius, const int tile_boundary, const int region_boundary, const int color_scheme, const string & file_name)
{
    ofstream output(file_name.c_str());

    if(region.size() != 4)
    {
        throw Exception("FrameBuffer::WriteFIG(): region.size() != 4");
        return 0;
    }

    if((color_scheme < 0) || (color_scheme > 1))
    {
        throw Exception("FrameBuffer::WriteFIG(): wrong color scheme");
        return 0;
    }

    if(!output)
    {
        throw Exception("FrameBuffer::WriteFIG(): null output");
        return 0;
    }
    else
    {
        // from http://www.xfig.org/userman/fig-format.html
        const int units = 1200;
        const int point_unit = 75;
        
        output << "#FIG 3.2" << endl;
        output << "Landscape" << endl;
        output << "Center" << endl;
        output << "Inches" << endl;
        output << "Letter" << endl;
        output << "100.00" << endl;
        output << "Single" << endl;
        output << "-2" << endl;
        output << units << " 2" << endl;

        // custom colors
        for(unsigned int i = 0; i < palette.size(); i++)
        {
            output << 0 << " " << 32+i << " #" << FigColorString(palette[i]) << endl;
        }

        float x_min = 1.0/sin(0.0);
        float x_max = -1.0/sin(0.0);
        float y_min = 1.0/sin(0.0);
        float y_max = -1.0/sin(0.0);
       
        for(unsigned int i = 0; i < samples.size(); i++)
        {
            // ellipse
            output << 1 << " ";

            // circle defined by radius
            output << 3 << " ";
            
            // line type
            output << 0 << " ";
            
            // thickness
            output << 0 << " ";
            
            // pen color black
            output << 7*color_scheme << " ";
            
            // fill color custom
            output << 32+samples[i].color_index << " ";

            // depth
            // output << 50 << " ";
            // output << (static_cast<int>(floor(Random::UniformRandom()*palette.size()))%palette.size()) + 50 - (palette.size()-1)/2<< " ";
            const int depth_mean = 50;
            const int depth_var = min(depth_mean, static_cast<int>(palette.size()));
            output << (static_cast<int>(floor(Random::UniformRandom()*depth_var))%depth_var) + depth_mean - (depth_var-1)/2 << " ";

            // line style default
            output << -1 << " ";

            // area fill full saturation
            output << 20 << " ";

            // style value
            output << 0.0 << " ";

            // direction
            output << 1 << " ";

            // angle of the x-axis
            output << 0.0 << " ";

            // center x, y
            const float center_x = floor(samples[i].x*units+0.5);
            const float center_y = floor(samples[i].y*units+0.5);

            output << center_x << " " << center_y << " ";

            if(center_x < x_min) x_min = center_x;
            if(center_x > x_max) x_max = center_x;
            if(center_y < y_min) y_min = center_y;
            if(center_y > y_max) y_max = center_y;

            // radius x, y
            const float radius_value = floor(radius*units+0.5);
            output << radius_value << " " << radius_value << " ";

            // the first point entered
            output << center_x << " " << center_y << " ";
            
            // the last point entered
            output << center_x+point_unit << " " << center_y+point_unit << " ";
            // done
            output << endl;
        }

        for(unsigned int i = 0; i < tiles.size(); i++)
        {
            if(tiles[i].vertices.size() <= 0) continue;

            // polyline
            output << 2 << " ";

            // polygon
            output << 3 << " ";

            // line style
            output << 0 << " ";

            // thickness
            output << max(1, tile_boundary) << " ";

            const int fill_color_index = 32 + tiles[i].color_index;

            // pen color black
            output << (tile_boundary ? 7*color_scheme : fill_color_index)<< " ";

            // fill color
            output << fill_color_index << " ";

            // depth
            output << _FIG_MAX_DEPTH << " ";

            // pen style
            output << -1 << " ";

            // area fill
            output << -1 << " ";

            // style value
            output << 0.0 << " ";

            // joint style
            output << 1 << " ";

            // cap style
            output << 1 << " ";

            // radiius
            output << -1 << " ";

            // forward arrow
            output << 0 << " ";

            // backward arrow
            output << 0 << " ";

            // npoints
            output << tiles[i].vertices.size()+1 << " " << endl;

            // the corners with the first one repeated
            output << "\t ";
            for(unsigned int j = 0; j <= tiles[i].vertices.size(); j++)
            {
                output << static_cast<int>(floor(tiles[i].vertices[j%tiles[i].vertices.size()].x*units+0.5)) << " " << static_cast<int>(floor(tiles[i].vertices[j%tiles[i].vertices.size()].y*units+0.5)) << " ";
            }

            output << endl;
        }

        for (unsigned int i = 0; i < circles.size(); ++i)
        {
            // ellipse
            output << 1 << " ";

            // circle defined by radius
            output << 3 << " ";

            // line type
            output << 0 << " ";

            // thickness
            output << max(1, tile_boundary) << " ";

            // pen color black
            output << 32+circles[i].color_index << " ";

            // fill color custom
            output << 32+circles[i].color_index << " ";

            // depth
            // output << 50 << " ";
            // output << (static_cast<int>(floor(Random::UniformRandom()*palette.size()))%palette.size()) + 50 - (palette.size()-1)/2<< " ";
            const int depth_mean = 50;
            const int depth_var = min(depth_mean, static_cast<int>(palette.size()));
            output << (static_cast<int>(floor(Random::UniformRandom()*depth_var))%depth_var) + depth_mean - (depth_var-1)/2 << " ";

            // line style default
            output << 0 << " ";

            // area fill full saturation
            output << -1 << " ";

            // style value
            output << 0.0 << " ";

            // direction
            output << 1 << " ";

            // angle of the x-axis
            output << 0.0 << " ";

            // center x, y
            const float center_x = floor(circles[i].x*units+0.5);
            const float center_y = floor(circles[i].y*units+0.5);

            output << center_x << " " << center_y << " ";

            // radius x, y
            const float radius_value = floor(circles[i].radius*units+0.5);
            output << radius_value << " " << radius_value << " ";

            // the first point entered
            output << center_x << " " << center_y << " ";

            // the last point entered
            output << center_x+point_unit << " " << center_y+point_unit << " ";
            // done
            output << endl;
        }

        if((region[0] < region[1]) && (region[2] < region[3]))
        {
            x_min = region[0]*units;
            x_max = region[1]*units;
            y_min = region[2]*units;
            y_max = region[3]*units;
        }

        {
            const float radius_value = floor(radius*units+0.5);
            x_min -= radius_value;
            x_max += radius_value;
            y_min -= radius_value;
            y_max += radius_value;
        }

        if(region_boundary || (!region_boundary && (color_scheme == 1)))
        {
            // polyline
            output << 2 << " ";

            // box
            output << 2 << " ";

            // line style
            output << 0 << " ";

            // thickness
            output << region_boundary << " ";

            // pen color black
            output << 7*color_scheme << " ";

            // fill color white
            output << 7*(1-color_scheme) << " ";

            // depth
            output << (tiles.size() <= 0 ? _FIG_MAX_DEPTH : _FIG_MAX_DEPTH-1)<< " ";
            
            // pen style
            output << -1 << " ";

            // area fill
            output << (tiles.size() <= 0 ? 20 : -1)<< " ";

            // style value
            output << 0.0 << " ";

            // joint style
            output << 0 << " ";

            // cap style
            output << 0 << " ";

            // radiius
            output << -1 << " ";

            // forward arrow
            output << 0 << " ";

            // backward arrow
            output << 0 << " ";

            // npoints
            output << 5 << " " << endl;

            // the 4 corners with the first one repeated
            output << "\t ";
            output << floor(x_min) << " " << floor(y_min) << " ";
            output << ceil(x_max) << " " << floor(y_min) << " ";
            output << ceil(x_max) << " " << ceil(y_max) << " ";
            output << floor(x_min) << " " << ceil(y_max) << " ";
            output << floor(x_min) << " " << floor(y_min) << " ";

            output << endl;
        }

        return 1;
    }
}

int FrameBuffer::WriteFIG(const vector<L2F> & samples, const vector<Picture> & palette, const vector<float> & region, const int boundary, const int color_scheme, const string & file_name)
{
    if(region.size() != 4)
    {
        return 0;
    }

    if((color_scheme < 0) || (color_scheme > 1))
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
        // from http://www.xfig.org/userman/fig-format.html
        const int units = 1200;
        const int point_unit = 75;
        
        output << "#FIG 3.2" << endl;
        output << "Landscape" << endl;
        output << "Center" << endl;
        output << "Inches" << endl;
        output << "Letter" << endl;
        output << "100.00" << endl;
        output << "Single" << endl;
        output << "-2" << endl;
        output << units << " 2" << endl;

        vector<L2F> vertices(4);

        for(unsigned int i = 0; i < samples.size(); i++)
        {
            const Picture & picture = palette[samples[i].color_index%palette.size()];

            for(unsigned int j = 0; j < vertices.size(); j++)
            {
                vertices[j].x = samples[i].x + ((j%3 != 0) - 0.5)*picture.width;
                vertices[j].y = samples[i].y + ((j/2) - 0.5)*picture.height;
            }

            // polyline
            output << 2 << " ";

            // imported picture bounding box
            output << 5 << " ";

            // line style
            output << 0 << " ";

            // thickness
            output << 1 << " ";

            // pen color black
            output << 0 << " ";

            // fill color white
            output << -1 << " ";

            // depth
            // output << (static_cast<int>(floor(Random::UniformRandom()*palette.size()))%palette.size()) + 50 - (palette.size()-1)/2 << " ";
            const int depth_mean = 50;
            const int depth_var = min(depth_mean, static_cast<int>(palette.size()));
            output << (static_cast<int>(floor(Random::UniformRandom()*depth_var))%depth_var) + depth_mean - (depth_var-1)/2 << " ";

            // pen style
            output << -1 << " ";

            // area fill
            output << -1 << " ";

            // style value
            output << 0.0 << " ";

            // joint style
            output << 0 << " ";

            // cap style
            output << 0 << " ";

            // radiius
            output << -1 << " ";

            // forward arrow
            output << 0 << " ";

            // backward arrow
            output << 0 << " ";

            // npoints
            output << (vertices.size() + 1) << " " << endl;

            // flipped
            output << "\t ";
            output << 0 << " ";

            // file name
            output << picture.name << " " << endl;

            // the 4 corners with the first one repeated
            output << "\t ";
            for(unsigned int j = 0; j <= vertices.size(); j++)
            {
                output << floor(vertices[j%vertices.size()].x*units+0.5) << " " << floor(vertices[j%vertices.size()].y*units+0.5) << " ";
            }

            output << endl;
        }
    }

    return 1;
}

string FrameBuffer::FigColorString(const P3 input)
{
    ostringstream output;
    output.width(2); output.fill('0');
    output << hex << (input.r%256);
    output.width(2); output.fill('0');
    output << hex << (input.g%256);
    output.width(2); output.fill('0');
    output << hex << (input.b%256);

    return output.str();
}

const int FrameBuffer::_FIG_MAX_DEPTH = 999;
