/*
  Seam.cpp

  visualize how seamster behavior

  Li-Yi Wei
  April 24, 2018

*/


#include <iostream>
using namespace std;

#include "Utility.hpp"
#include "FrameBuffer.hpp"
#include "Texture.hpp"
#include "SequentialCounter.hpp"
#include "Seamster.hpp"
#include "Exception.hpp"

int Main(int argc, char **argv)
{
    // input arguments
    if(argc < 5)
    {
        cerr << "Usage: " << argv[0] << " input_image_file_path input_boundary (toroidal or none) neighborhood_spec (e.g. square1) output_image_file_path" << endl;
        return 1;
    }
    
    int arg_ctr = 0;
    
    const string input_image_file_path(argv[++arg_ctr]);
    const string input_boundary(argv[++arg_ctr]);
    const string output_boundary("toroidal");
    const string neighborhood_spec(argv[++arg_ctr]);
    const string output_image_file_path(argv[++arg_ctr]);

    // input image
    Array<FrameBuffer::P3> input_image;
    int max_input_value = -1;
    if(! FrameBuffer::ReadPPM(input_image_file_path, input_image, max_input_value))
    {
        cerr << "cannot read from " << input_image_file_path << endl;
        return 1;
    }

    const Texture input_texture(input_image);
    
    const int dimension = input_texture.Dimension();

    // domain and neighborhood
    const shared_ptr<Domain> p_input_domain = Utility::BuildDomain(input_boundary);
    const shared_ptr<Domain> p_output_domain = Utility::BuildDomain(output_boundary);

    if(!p_input_domain || !p_output_domain)
    {
        cerr << "null domain" << endl;
        return 1;
    }

    const Domain & input_domain = *p_input_domain;
    const Domain & output_domain = *p_output_domain;

    const shared_ptr<Templar> p_templar = Utility::BuildTemplar(neighborhood_spec, dimension);
    if(!p_templar)
    {
        cerr << "null templar" << endl;
        return 1;
    }

    const shared_ptr<Neighborhood> p_output_neighborhood = Utility::BuildNeighborhood(*p_templar, output_domain);

    const Neighborhood & output_neighborhood = *p_output_neighborhood;

    // output texture
    const Texture output_texture(input_texture);
    Array<FrameBuffer::P3> output_image(input_image);

    // seamster
    Seamster seamster;

    SequentialCounter counter(dimension, vector<int>(dimension, 0), Utility::Minus1(output_image.Size()));
    counter.Reset();
    vector<int> index(dimension);
    FrameBuffer::P3 pixel;

    do
    {
        if(! counter.Get(index))
        {
            cerr << "cannot get index" << endl;
            return 1;
        }

        const bool at_seam = seamster.AtSeam(input_texture, input_domain, output_texture, output_neighborhood, index);

        pixel.r = pixel.g = pixel.b = at_seam*max_input_value;
        if(! output_image.Put(index, pixel))
        {
            cerr << "cannot put pixel" << endl;
            return 1;
        }
    }
    while(counter.Next());

    // output
    if(! FrameBuffer::WritePPM(output_image, max_input_value, output_image_file_path))
    {
        cerr << "cannot write to " << output_image_file_path << endl;
        return 1;
    }

    // done
    return 0;
}

int main(int argc, char **argv)
{
    try
    {
        return Main(argc, argv);
    }   
    catch(Exception e)
    {   
        cout << e.Message() << endl;
        return 1;
    }
}

