/*
 * FrameBuffer.hpp
 *
 * some commom frame buffer operations
 *
 * Li-Yi Wei
 *
 */

#ifndef _FRAME_BUFFER_HPP
#define _FRAME_BUFFER_HPP

#include <vector>
#include <string>
using namespace std;

#include "Array.hpp"

class FrameBuffer
{
public:
    typedef int P2;

    static int ReadPGM(const string & file_name,
                       Array<P2> & pixels,
                       int & maximum_value);
    
    static int WritePGM(const Array<P2> & pixels,
                        const int maximum_value,
                        const string & file_name);

    struct P3
    {
        int r, g, b;
    };

    static int ReadPPM(const string & file_name,
                       Array<P3> & pixels,
                       int & maximum_value);
    
    static int WritePPM(const Array<P3> & pixels,
                        const int maximum_value,
                        const string & file_name);

    typedef float F2;

    static int ReadF2M(const string & file_name,
                       Array<F2> & pixels);
    
    static int WriteF2M(const Array<F2> & pixels,
                        const string & file_name);

    // binary mode from Rui
    static int ReadF2MB(const string & file_name,
                        Array<F2> & pixels);

    struct PF
    // union PF // assume monochrome only now
    {
        float r;
        float g;
        float b;
    };

    static int ReadPFM(const string & file_name,
                       Array<PF> & pixels);
    
    static int WritePFM(const Array<PF> & pixels,
                        const string & file_name);

    struct L2F
    {
        int color_index;
        float x, y;
    };

    // color_scheme: 0 for black over white, 1 vice versa
    // region: [x_min x_max y_min y_max] in float
    static int WriteSVG(const vector<L2F> & samples, const vector<P3> & palette, const vector<float> & region, const float radius, const int boundary, const int color_scheme, const string & file_name);

    // color_scheme: 0 for black over white, 1 vice versa
    // region: [x_min x_max y_min y_max] in float
    static int WriteFIG(const vector<L2F> & samples, const vector<P3> & palette, const vector<float> & region, const float radius, const int boundary, const int color_scheme, const string & file_name);
 
    struct Tile
    {
        int color_index;
        vector<L2F> vertices;
    };

    static int WriteFIG(const vector<Tile> & tiles, const vector<P3> & palette, const vector<float> & region, const int tile_boundary, const int region_boundary, const int color_scheme, const string & file_name);

    struct Circle
    {
        int color_index;
        float x, y, radius;
    };

    static int WriteFIG(const vector<Circle> & circles, const vector<P3> & palette, const vector<float> & region, const int tile_boundary, const int region_boundary, const int color_scheme, const string & file_name);

    static int WriteFIG(const vector<L2F> & samples, const vector<Tile> & tiles, const vector<Circle> & circles, const vector<P3> & palette, const vector<float> & region, const float sample_radius, const int tile_boundary, const int region_boundary, const int color_scheme, const string & file_name);

    struct Picture
    {
        string name;
        float width, height;
    };

    static int WriteFIG(const vector<L2F> & samples, const vector<Picture> & palette, const vector<float> & region, const int boundary, const int color_scheme, const string & file_name);

protected:
    static string FigColorString(const P3 input);
    static const int _FIG_MAX_DEPTH;
};

#endif
