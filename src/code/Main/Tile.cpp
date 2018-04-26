/*
  Tile.cpp

  tileable image generation

  Li-Yi Wei
  April 25, 2018

*/

#include <iostream>
using namespace std;

#include "Texture.hpp"
#include "VectorRange.hpp"

#include "FrameBuffer.hpp"
#include "Random.hpp"
#include "Utility.hpp"
#include "Exception.hpp"

int Main(int argc, char **argv)
{
    // input arguments
    if(argc < 7)
    {
        cerr << "Usage: " << argv[0] << " input_image_file_path input_boundary (toroidal or none)  synthesizer_spec sequencer_spec neighborhood_spec (e.g. square1) output_image_file_path" << endl;
        return 1;
    }
    
    int arg_ctr = 0;
    
    const string input_image_file_path(argv[++arg_ctr]);
    const string input_boundary(argv[++arg_ctr]);
    const string output_boundary("toroidal");
    const string synthesis_spec(argv[++arg_ctr]);
    const string sequence_spec(argv[++arg_ctr]);
    const string neighborhood_spec(argv[++arg_ctr]);
    const string output_image_file_path(argv[++arg_ctr]);

    // input image
    Array<FrameBuffer::P3> input_image;
    int max_pixel_value = -1;
    if(! FrameBuffer::ReadPPM(input_image_file_path, input_image, max_pixel_value))
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

    // range
    const int range_dimension = 3; // RGB

    const RangePtr penalty_range(new VectorRange<int>(vector<int>(range_dimension, max_pixel_value)));
    const RangePtr zero_range(new VectorRange<int>(vector<int>(range_dimension, 0)));

    // initialization
    Random::InitRandomNumberGenerator();

    // output texture
    Texture output_texture(input_texture);

    // synthesis
    const string message = Utility::Synthesize(input_boundary, output_boundary, synthesis_spec, sequence_spec, neighborhood_spec, input_texture, output_texture, penalty_range, zero_range);

    if(message != "")
    {
        cerr << "error in synthesis: " << message << endl;
        return 1;
    }

    // output
    Array<FrameBuffer::P3> output_image;
    if(!output_texture.Convert(output_image))
    {
        cerr << "cannot convert output" << endl;
        return 1;
    }

    if(! FrameBuffer::WritePPM(output_image, max_pixel_value, output_image_file_path))
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

