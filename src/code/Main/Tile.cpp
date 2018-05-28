/*
  Tile.cpp

  tileable image generation

  Li-Yi Wei
  April 25, 2018

*/

#include <iostream>
#include <string>
using namespace std;

#include "Texture.hpp"
#include "VectorRange.hpp"

#include "SequentialCounter.hpp"
#include "FrameBuffer.hpp"
#include "Random.hpp"
#include "Utility.hpp"
#include "Exception.hpp"

string Convert(const vector<int> & output_size, Array<Position> & coord_array)
{
    coord_array = Array<Position>(output_size);

    SequentialCounter counter(output_size.size(), vector<int>(output_size.size(), 0), Utility::Minus1(output_size));
    
    vector<int> index;
    counter.Reset();
        
    do
    {
        counter.Get(index);

        if(! coord_array.Put(index, index))
        {
            return "cannot put position";
        }
    }
    while(counter.Next());

    // done
    return "";
}

string Convert(const Array<FrameBuffer::P3> & coord_image, Array<Position> & coord_array)
{
    const vector<int> output_size = coord_image.Size();
    coord_array = Array<Position>(output_size);

    SequentialCounter counter(output_size.size(), vector<int>(output_size.size(), 0), Utility::Minus1(output_size));
    
    FrameBuffer::P3 pixel;
    Position position(2);
    vector<int> index;
    counter.Reset();
        
    do
    {
        counter.Get(index);
      
        if(! coord_image.Get(index, pixel))
        {
            return "cannot get pixel";
        }

        position[0] = pixel.g;
        position[1] = pixel.r;

        if(! coord_array.Put(index, position))
        {
            return "cannot put position";
        }
    }
    while(counter.Next());

    // done
    return "";
}

int Main(int argc, char **argv)
{
    // input arguments
    if(argc < 8)
    {
        cerr << "Usage: " << argv[0] << " input_image_file_path init_coord_spec input_boundary (toroidal or none)  synthesizer_spec sequencer_spec neighborhood_spec (e.g. square1) output_image_file_path" << endl;
        return 1;
    }
    
    int arg_ctr = 0;
    
    const string input_image_file_path(argv[++arg_ctr]);
    const string init_coord_spec(argv[++arg_ctr]);
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

    // init coord
    Array<Position> init_coord;
    {
        Array<FrameBuffer::P3> init_coord_image;
        int max_coord_value = -1;

        string message;
        if(FrameBuffer::ReadPPM(init_coord_spec, init_coord_image, max_coord_value))
        {
            message = Convert(init_coord_image, init_coord);
        }
        else
        {
            message = Convert(input_image.Size(), init_coord);
        }
    
        if(message != "")
        {
            cerr << "error in converting coord image: " << message << endl;
            return 1;
        }
    }

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
    Texture output_texture(input_texture, init_coord);

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

