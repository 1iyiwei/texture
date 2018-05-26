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
};

#endif
