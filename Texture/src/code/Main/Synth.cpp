/*
  Synth.cpp

  main texture synthesis

  Li-Yi Wei
  August 18, 2014

*/

#include <iostream>
#include <string>
using namespace std;

#include "Texture.hpp"
#include "VectorRange.hpp"
#include "FrameBuffer.hpp"

#include "Random.hpp"

#include "Utility.hpp"
#include "PlainPyramidDomain.hpp"
#include "Exception.hpp"

int Main(int argc, char **argv)
{
    // input arguments

    const int argc_min = 9;
    if(argc < argc_min)
    {
        cerr << "Usage: " << argv[0] << " input_file_name input_boundary output_boundary synthesizer_spec sequencer_spec neighborhood_spec output_size (heightxwidth) output_file_name [constraint_file_name]" << endl;
        return 1;
    }

    int arg_ctr = 0;
    const string input_file_name(argv[++arg_ctr]);
    const string input_boundary(argv[++arg_ctr]);
    const string output_boundary(argv[++arg_ctr]);
    const string synthesis_spec(argv[++arg_ctr]);
    const string sequence_spec(argv[++arg_ctr]);
    const string neighborhood_spec(argv[++arg_ctr]);
    const string output_size_spec(argv[++arg_ctr]);
    const string output_file_name(argv[++arg_ctr]);
    const string constraint_file_name = ((arg_ctr+1) < argc ? argv[++arg_ctr] : "");

    const vector<int> output_size = Utility::ExtractIntegers(output_size_spec);

    // input
    TexturePyramidPtr input_pyramid_ptr;
    int maximum_value = 0;
    {
        string message = Utility::ReadPyramid(input_file_name, input_pyramid_ptr, maximum_value);
        if(message != "")
        {
            cerr << "cannot read from " << input_file_name << " because " << message << endl;
            return 1;
        }
    }

    if(input_pyramid_ptr == 0)
    {
        cerr << "null input pyramid" << endl;
        return 1;
    }

    const TexturePyramid & input_pyramid(*input_pyramid_ptr);

    if(input_pyramid.Size() < 1)
    {
        cerr << "empty input pyramid" << endl;
        return 1;
    }

    const Texture & input = *(input_pyramid.Get(0));

    const int range_dimension = 3; 

    const RangePtr penalty_range(new VectorRange<int>(vector<int>(range_dimension, maximum_value)));
    const RangePtr zero_range(new VectorRange<int>(vector<int>(range_dimension, 0)));

    // initialization
    Random::InitRandomNumberGenerator();

    TexturePyramid output_pyramid(output_size, input_pyramid.Size());

    Texture & output = *(output_pyramid.Get(0));

    // constraint, if any
    vector<TexelPtr> constraints;

    if(constraint_file_name != "")
    {
        const int domain_dimension = output.Dimension();

        string message = Utility::ReadConstraints(constraint_file_name, domain_dimension, range_dimension, constraints);

        if(message != "")
        {
            cerr << "error in reading constraints: " << message << endl;
            return 1;
        }

        cout << constraints.size() << " constraints read" << endl;

        PlainPyramidDomain pyramid_domain; // hard code for now as this is the only choice; 

        message = Utility::HardConstrain(constraints, pyramid_domain, input_pyramid, output_pyramid);

        if(message != "")
        {
            cerr << "error in applying constraints: " << message << endl;
            return 1;
        }
    }

    // diffusion seeds
    vector<Position> diffusion_seeds(constraints.size());
    for(unsigned int i = 0; i < diffusion_seeds.size(); i++)
    {
        diffusion_seeds[i] = constraints[i]->GetPosition();
    }

    vector< vector<Position> > pyramid_diffusion_seeds;
    {
        const string message = Utility::GetConstraints(output_pyramid, pyramid_diffusion_seeds);
        if(message != "")
        {
            cerr << "error in get constraints: " << message << endl;
            return 1;
        }
    }

    // synthesis
    const string message = Utility::Synthesize(input_boundary, output_boundary, synthesis_spec, sequence_spec, neighborhood_spec, input_pyramid, output_pyramid, penalty_range, zero_range, pyramid_diffusion_seeds);

    if(message != "")
    {
        cerr << "error in synthesis: " << message << endl;
        return 1;
    }

    // output 
    Array<FrameBuffer::P3> image;
    if(! output.Convert(image))
    {
        cerr << "cannot convert output" << endl;
        return 1;
    }

    if(! FrameBuffer::WritePPM(image, maximum_value, output_file_name))
    {
        cerr << "cannot write to " << output_file_name << endl;
        return 1;
    }

    // check constraints
    for(unsigned int k = 0; k < constraints.size(); k++)
    {
        const Position position_in = constraints[k]->GetPosition();
        const VectorRange<int> * range_in = dynamic_cast<VectorRange<int> *>(constraints[k]->GetRange().get());

        TexelPtr texel_ptr;
        if(! output.Get(position_in, texel_ptr))
        {
            cerr << "cannot read back constrained position" << endl;
            return 1;
        }
        const Position position_out = texel_ptr->GetPosition();
        const VectorRange<int> * range_out = dynamic_cast<VectorRange<int> *>(texel_ptr->GetRange().get());

        cout << "[";
        for(unsigned int i = 0; i < position_in.size(); i++)
        {
            cout << position_in[i];
            if((i+1) < position_in.size()) cout << ", ";
        }
        cout << "] - [";
        for(unsigned int i = 0; i < position_out.size(); i++)
        {
            cout << position_out[i];
            if((i+1) < position_out.size()) cout << ", ";
        }
        cout << "] : (";
        int value = 0;
        for(int i = 0; i < range_in->Length(); i++)
        {
            range_in->Get(i, value);
            cout << value;
            if((i+1) < range_in->Length()) cout << ", ";
        }
        cout << ") - (";
        for(int i = 0; i < range_out->Length(); i++)
        {
            range_out->Get(i, value);
            cout << value;
            if((i+1) < range_out->Length()) cout << ", ";
        }
        cout << ")" << endl;
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

