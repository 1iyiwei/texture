/*
  Utility.cpp

  Li-Yi Wei
  August 17, 2014

*/

#include <fstream>
#include <sstream>
using namespace std;

#include "Utility.hpp"
#include "Random.hpp"

#include "VectorRange.hpp"

#include "SquareTemplate.hpp"
#include "CausalTemplate.hpp"

#include "ToroidalDomain.hpp"
#include "PlainDomain.hpp"

#include "PlainPyramidDomain.hpp"

#include "PlainNeighborhood.hpp"
#include "PyramidNeighborhood.hpp"

#include "RandomSynthesizer.hpp"
#include "CoherenceSynthesizer.hpp"

#include "ScanlineSequencer.hpp"
#include "RandomVisitSequencer.hpp"
#include "RandomShuffleSequencer.hpp"
#include "RandomDiffusionSequencer.hpp"

#include "SequentialCounter.hpp"

vector<int> Utility::Minus1(const vector<int> & input)
{
    vector<int> output(input);

    for(unsigned int i = 0; i < output.size(); i++)
    {
        output[i] = input[i]-1;
    }

    return output;
}

vector<string> & Utility::Split(const string & input, const char delim, vector<string> & output)
{
    stringstream ss(input);
    string item;
    output.clear();

    while(std::getline(ss, item, delim)) 
    {
        output.push_back(item);
    }

    // remove empty tokens
    {
        vector<string> non_empty_tokens;
        for(unsigned k = 0; k < output.size(); k++)
        {
            if(output[k] != "")
            {
                non_empty_tokens.push_back(output[k]);
            }
        }

        output = non_empty_tokens;
    }

    return output;
}

vector<string> Utility::Split(const string & input, char delim)
{
    vector<string> output;
    Split(input, delim, output);
    return output;
}

vector<int> Utility::ExtractIntegers(const string & input)
{
    // get tokens
    string purified(input);

    char splitter = 'x';

    for(unsigned int k = 0; k < purified.size(); k++)
    {
        if((purified[k] < '0') || (purified[k] > '9'))
        {
            purified[k] = splitter;
        }
    }

    vector<string> tokens = Split(purified, splitter);

    // conversion
    vector<int> output;
    for(unsigned int i = 0; i < tokens.size(); i++)
    {
        output.push_back(atoi(tokens[i].c_str()));
    }

    return output;
}

int Utility::ExtractInteger(const string & input, const int default_value)
{
    vector<int> values = ExtractIntegers(input);

    return (values.size() > 0 ? values[0] : default_value);
}

string Utility::ReadPyramid(const string & pyramid_spec, TexturePyramidPtr & pyramid, int & maximum_value)
{
    const vector<string> level_file_names = Split(pyramid_spec, _PYRAMID_SEPARATOR);

    vector<TexturePtr> textures(level_file_names.size());

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        const string & input_file_name = level_file_names[i];

        Array<FrameBuffer::P3> image;
        if(! FrameBuffer::ReadPPM(input_file_name, image, maximum_value))
        {
            return "cannot read from " + input_file_name;
        }

        textures[i].reset(new Texture(image));
    }

    pyramid.reset(new TexturePyramid(textures));

    return "";
}

shared_ptr<Template> Utility::BuildTemplate(const string & neighborhood_spec, const int dimension)
{
    const vector<int> values = ExtractIntegers(neighborhood_spec);

    return BuildTemplate(neighborhood_spec, values, dimension);
}

shared_ptr<Template> Utility::BuildTemplate(const string & neighborhood_spec, const vector<int> & values, const int dimension)
{
    shared_ptr<Template> templar;

    if(neighborhood_spec.find("square") != string::npos)
    {
        if(values.size() == 1)
        {
            const int half_size = values[0];
            templar.reset(new SquareTemplate(dimension, half_size));
        }
    }
    else if(neighborhood_spec.find("sphere") != string::npos)
    {
        if(values.size() == 1)
        {
            const int radius = values[0];
            // not yet done
            // templar.reset(new SphereTemplate(radius));
        }
    }
    else
    {
        // unknown template
    }

    return templar;
}

shared_ptr<Domain> Utility::BuildDomain(const string & boundary_condition)
{
    shared_ptr<Domain> domain;

    if(boundary_condition == "toroidal")
    {
        domain.reset(new ToroidalDomain());
    }
    else
    {
        domain.reset(new PlainDomain());
    }

    return domain;
}

shared_ptr<Neighborhood> Utility::BuildNeighborhood(const Template & templar, const Domain & domain)
{
    shared_ptr<Neighborhood> neighborhood(new PlainNeighborhood(templar, domain));

    return neighborhood;
}

shared_ptr<Neighborhood> Utility::BuildNeighborhood(const vector<TemplatePtr> & templars, const Domain & domain, const TexturePyramid & pyramid, const PyramidDomain & pyramid_domain)
{
    shared_ptr<Neighborhood> neighborhood(new PyramidNeighborhood(pyramid, pyramid_domain, templars, domain));

    return neighborhood;
}

shared_ptr<Sequencer> Utility::BuildSequencer(const string & sequence_spec, const vector<Position> & diffusion_seeds, const Neighborhood & neighborhood)
{
    shared_ptr<Sequencer> sequencer;

    if(sequence_spec.find("scanline") != string::npos)
    {
        sequencer.reset(new ScanlineSequencer());
    } 
    else if(sequence_spec.find("randomvisit") != string::npos)
    {
        sequencer.reset(new RandomVisitSequencer());
    }
    else if(sequence_spec.find("randomshuffle") != string::npos)
    {
        sequencer.reset(new RandomShuffleSequencer());
    }
    else if(sequence_spec.find("randomdiffusion") != string::npos)
    {
        sequencer.reset(new RandomDiffusionSequencer(diffusion_seeds, neighborhood));
    }
    else if(sequence_spec.find("random") != string::npos)
    {
        sequencer.reset(new RandomShuffleSequencer());
    }
    else
    {
        // unknown sequencer mode
    }

    return sequencer;
}

string Utility::ReadConstraints(const string & file_name, const int domain_dimension, const int range_dimension, vector<TexelPtr> & constraints)
{
    ifstream input(file_name.c_str());

    if(!input)
    {
        return "cannot read from " + file_name;
    }

    constraints.clear();

    while(input.good())
    {
        Position position(domain_dimension);
        vector<int> range(range_dimension, 0);

        for(unsigned int k = 0; k < position.size(); k++)
        {
            input >> position[k];
        }

        for(unsigned int k = 0; k < range.size(); k++)
        {
            input >> range[k];
        }

        if(! (input.bad() || input.fail()))
        {
            RangePtr range_ptr(new VectorRange<int>(range));
            TexelPtr texel(new Texel(range_ptr, position));
            constraints.push_back(texel);
        }
    }

    // done
    return "";
}

TexelPtr Utility::SatisfyConstraint(const TexelPtr & constraint, const vector<TexelPtr> & candidates)
{
    TexelPtr answer;

    const int start = Random::UniformInt(0, candidates.size()-1);

    for(unsigned int k = 0; k < candidates.size(); k++)
    {
        const int index = (k+start)%candidates.size();

        const TexelPtr & candidate = candidates[index];

        const RangePtr constraint_range = constraint->GetRange();
        const RangePtr candidate_range = candidate->GetRange();

        if(constraint_range->Distance2(*candidate_range) <= 0)
        {
            answer = candidate;
            break;
        }
    }

    return answer;
}

string Utility::HardConstrain(const vector<TexelPtr> & constraints, const Texture & source, Texture & target)
{
    vector<TexelPtr> candidates;
    source.Get(candidates);

    for(unsigned int k = 0; k < constraints.size(); k++)
    {
        if(! constraints[k])
        {
            return "empty constraint";
        }

        TexelPtr candidate = SatisfyConstraint(constraints[k], candidates);

        if(! candidate)
        {
            return "constraint does not exist in the source";
        }

        TexelPtr texel(new Texel(*candidate));
        texel->SetStatus(Texel::FIXED);

        if(! target.Put(constraints[k]->GetPosition(), texel))
        {
            return "cannot set target constraint";
        }
    }

    // done
    return "";
}

string Utility::HardConstrain(const vector<TexelPtr> & constraints, const PyramidDomain & pyramid_domain, const TexturePyramid & source_pyramid, TexturePyramid & target_pyramid)
{
    if(source_pyramid.Size() != target_pyramid.Size())
    {
        return "incompatible source and target pyramids";
    }

    vector<TexelPtr> candidates;
    {
        TexturePtr source_ptr = source_pyramid.Get(0);
        if(source_ptr == 0) return "empty source";

        source_ptr->Get(candidates);
    }

    for(unsigned int k = 0; k < constraints.size(); k++)
    {
        if(! constraints[k])
        {
            return "empty constraint";
        }

        const TexelPtr candidate = SatisfyConstraint(constraints[k], candidates);

        if(! candidate)
        {
            return "constraint does not exist in the source";
        }

        Position source_position = candidate->GetPosition();
        Position target_position = constraints[k]->GetPosition();

        for(int level = 0; level < target_pyramid.Size(); level++)
        {
            // copy from source position to target position
            if(! pyramid_domain.Trace(source_pyramid.Size(0), candidate->GetPosition(), source_pyramid.Size(level), source_position))
            {
                return "cannot trace source pyramid domain";
            }

            if(! pyramid_domain.Trace(target_pyramid.Size(0), constraints[k]->GetPosition(), target_pyramid.Size(level), target_position))
            {
                return "cannot trace target pyramid domain";
            }

            TexturePtr source_ptr = source_pyramid.Get(level);
            if(source_ptr == 0) return "empty source";
            TexelPtr source_texel;
            if(! source_ptr->Get(source_position, source_texel))
            {
                return "cannot get source texel";
            }

            TexturePtr target_ptr = target_pyramid.Get(level);
            if(target_ptr == 0) return "empty target";
            TexelPtr texel(new Texel(*source_texel));
            texel->SetStatus(Texel::FIXED);
            if(! target_ptr->Put(target_position, texel))
            {
                return "cannot set target constraint";
            }
        }
    }

    // done
    return "";
}

string Utility::GetConstraints(const Texture & texture, vector<Position> & positions)
{
    positions.clear();
    const vector<int> & texture_size = texture.Size();

    SequentialCounter counter(texture_size.size(), vector<int>(texture_size.size(), 0), Minus1(texture_size));

    vector<int> index;
    counter.Reset();
    do
    {
        counter.Get(index);

        TexelPtr texel;
        if(! texture.Get(index, texel))
        {
            return "cannot get texel";
        }

        if(texel && (texel->GetStatus() == Texel::FIXED))
        {
            positions.push_back(index);
        }
    }
    while(counter.Next());

    return "";
}

string Utility::GetConstraints(const TexturePyramid & texture_pyramid, vector< vector<Position> > & positions)
{
    positions.clear();

    for(int level = 0; level < texture_pyramid.Size(); level++)
    {
        const TexturePtr level_ptr = texture_pyramid.Get(level);

        vector<Position> level_positions;

        if(level_ptr)
        {
            string message = GetConstraints(*level_ptr, level_positions);
            if(message != "") return message;
        }

        positions.push_back(level_positions);
    }

    return "";
}

#if 0
string Utility::Initialize(const string & input_boundary, const string & output_boundary, const string & neighborhood_spec, const Texture & source, Texture & target)
{
    RangePtr null_range;

    return SynthesizeOnce(input_boundary, output_boundary, "random", "scanline1", neighborhood_spec, source, target, null_range, null_range, vector<Position>(0));
}
#endif

char Utility::Separator(const SeparatorType type)
{
    char answer;

    switch(type)
    {
    case PASS:
        answer = _PASS_SEPARATOR;
        break;

    case LEVEL:
        answer = _LEVEL_SEPARATOR;
        break;

    case PYRAMID:
        answer = _PYRAMID_SEPARATOR;
        break;

    default:
        answer = _UNKNOWN_SEPARATOR;
        break;
    }

    return answer;
}

string Utility::Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const TexturePyramid & source, TexturePyramid & target, const RangePtr & penalty_range, const RangePtr & zero_range)
{
    vector< vector<Position> > empty_seeds;

    return Synthesize(input_boundary, output_boundary, synthesis_spec, sequence_spec, neighborhood_spec, source, target, penalty_range, zero_range, empty_seeds);
}

string Utility::Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const TexturePyramid & source, TexturePyramid & target, const RangePtr & penalty_range, const RangePtr & zero_range, const vector< vector<Position> > & diffusion_seeds)
{
    // check
    if(source.Size() != target.Size())
    {
        return "incompatible source and target pyramid heights";
    }

    if((diffusion_seeds.size() > 0) && (diffusion_seeds.size() != target.Size()))
    {
        stringstream message;
        message << "incompatible target and diffusion seeds: " << diffusion_seeds.size() << " != " << target.Size();
        return message.str();
    }

    // split
    const vector<string> synthesis_specs = Split(synthesis_spec, _PYRAMID_SEPARATOR);
    const vector<string> sequence_specs = Split(sequence_spec, _PYRAMID_SEPARATOR);
    const vector<string> neighborhood_specs = Split(neighborhood_spec, _PYRAMID_SEPARATOR);

    if(!LegalPyramidSpecSize(synthesis_specs.size(), source.Size()))
    {
        return "illegal synthesis spec";
    }
    if(!LegalPyramidSpecSize(sequence_specs.size(), source.Size()))
    {
        return "illegal sequence spec";
    }
    if(!LegalPyramidSpecSize(neighborhood_specs.size(), source.Size()))
    {
        return "illegal neighborhood spec";
    }

    // level by level synthesis
    for(int level = target.Size()-1; level >= 0; level--)
    {
        const Texture & source_texture = *(source.Get(level));
        Texture & target_texture = *(target.Get(level));

        TexturePyramidPtr source_pyramid(new TexturePyramid(source, level+1, source.Size()-1));
        TexturePyramidPtr target_pyramid(new TexturePyramid(target, level+1, target.Size()-1));

        const string message = Synthesize(input_boundary, output_boundary, ChooseSpec(synthesis_specs, level), ChooseSpec(sequence_specs, level), ChooseSpec(neighborhood_specs, level), source_texture, source_pyramid, target_texture, target_pyramid, penalty_range, zero_range, diffusion_seeds[level]);

        if(message != "")
        {
            return message;
        }
    }

    // done
    return "";
}

string Utility::Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, Texture & target, const RangePtr & penalty_range, const RangePtr & zero_range)
{
    const vector<Position> empty_seeds;

    return Synthesize(input_boundary, output_boundary, synthesis_spec, sequence_spec, neighborhood_spec, source, target, penalty_range, zero_range, empty_seeds);
}

string Utility::Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, Texture & target, const RangePtr & penalty_range, const RangePtr & zero_range, const vector<Position> & diffusion_seeds)
{
    TexturePyramidPtr null_pyramid;

    return Synthesize(input_boundary, output_boundary, synthesis_spec, sequence_spec, neighborhood_spec, source, null_pyramid, target, null_pyramid, penalty_range, zero_range, diffusion_seeds);
}

string Utility::Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, const TexturePyramidPtr & source_pyramid, Texture & target, const TexturePyramidPtr & target_pyramid, const RangePtr & penalty_range, const RangePtr & zero_range, const vector<Position> & diffusion_seeds)
{
    if(sequence_spec == "") return "";
    const vector<string> sequence_specs = Split(sequence_spec, _PASS_SEPARATOR);

    const vector<string> synthesis_specs = Split(synthesis_spec, _PASS_SEPARATOR);
    if(synthesis_specs.size() < 1)
    {
        return "empty synthesis spec";
    }

    const vector<string> neighborhood_specs = Split(neighborhood_spec, _PASS_SEPARATOR);
    if(neighborhood_specs.size() < 1)
    {
        return "empty neighborhood spec";
    }

    string message;

#if 0
    // better use explicit control from command line for better flexibility
    // initialization
    message = Initialize(input_boundary, output_boundary, neighborhood_specs[0], source, target);

    if(message != "")
    {
        return message;
    }
#endif

    // check
    if((synthesis_specs.size() > 1) && (synthesis_specs.size() != sequence_specs.size()))
    {
        return "incompatible synthesis_spec and sequence_spec";
    }

    if((neighborhood_specs.size() > 1) && (neighborhood_specs.size() != sequence_specs.size()))
    {
        return "incompatible neighborhood_spec and sequence_spec";
    }

    // synthesis
    for(unsigned int k = 0; k < sequence_specs.size(); k++)
    {
        message = SynthesizeOnce(input_boundary, output_boundary, ChooseSpec(synthesis_specs, k), sequence_specs[k], ChooseSpec(neighborhood_specs, k), source, source_pyramid, target, target_pyramid, penalty_range, zero_range, diffusion_seeds);

        if(message != "")
        {
            return message;
        }
    }

    // done
    return "";
}

string Utility::SynthesizeOnce(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, Texture & target, const RangePtr & penalty_range, const RangePtr & zero_range, const vector<Position> & diffusion_seeds)
{
    TexturePyramidPtr null_pyramid;

    return SynthesizeOnce(input_boundary, output_boundary, synthesis_spec, sequence_spec, neighborhood_spec, source, null_pyramid, target, null_pyramid, penalty_range, zero_range, diffusion_seeds);
}

string Utility::SynthesizeOnce(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, const TexturePyramidPtr & source_pyramid, Texture & target, const TexturePyramidPtr & target_pyramid, const RangePtr & penalty_range, const RangePtr & zero_range, const vector<Position> & diffusion_seeds)
{
    // neighborhood
    const vector<string> neighborhood_specs = Split(neighborhood_spec, _LEVEL_SEPARATOR);
    vector<TemplatePtr> templars;
    for(unsigned int level = 0; level < neighborhood_specs.size(); level++)
    {
        const string & level_spec = neighborhood_specs[level];
        TemplatePtr templar = BuildTemplate(level_spec, source.Dimension());
        if(! templar)
        {
            return "unknown neighborhood: " + level_spec;
        }

        if((level == 0) && (sequence_spec.find("scanline") != string::npos))
        {
            templar.reset(new CausalTemplate(*templar));
        }

        templars.push_back(templar);
    }

    if(templars.size() <= 0)
    {
        return "empty spec: " + neighborhood_spec;
    }

    shared_ptr<Domain> input_domain = BuildDomain(input_boundary);
    shared_ptr<Domain> output_domain = BuildDomain(output_boundary);

    PlainPyramidDomain pyramid_domain;

    shared_ptr<Neighborhood> input_neighborhood = (source_pyramid == 0 ? BuildNeighborhood(*templars[0], *input_domain) : BuildNeighborhood(templars, *input_domain, *source_pyramid, pyramid_domain));
    shared_ptr<Neighborhood> output_neighborhood = (target_pyramid == 0 ? BuildNeighborhood(*templars[0], *output_domain) : BuildNeighborhood(templars, *output_domain, *target_pyramid, pyramid_domain));
    
    // sequencer
    shared_ptr<Sequencer> sequencer = BuildSequencer(sequence_spec, diffusion_seeds, *output_neighborhood);

    if(!sequencer)
    {
        return "unknown sequencer mode: " + sequence_spec;
    }

    vector<int> sequence_values = ExtractIntegers(sequence_spec);
    if(sequence_values.size() != 1)
    {
        return "missing number of iterations from " + sequence_spec + " with number of values " + to_string(static_cast<unsigned long long>(sequence_values.size()));
    }

    const int num_iterations = sequence_values[0];

    // synthesis
    shared_ptr<Synthesizer> synthesizer;

    if(synthesis_spec.find("random") != string::npos)
    {
        synthesizer.reset(new RandomSynthesizer(source));

        return sequencer->Synthesize(*synthesizer, target);
    }
    else if(synthesis_spec.find("coherence") != string::npos)
    {
        vector<string> synthesis_specs = Split(synthesis_spec, _PARAM_GROUP_SEPARATOR);

        if(synthesis_specs.size() > 2)
        {
            return "unknown parameter grouping for coherence synthesis";
        }

        const int extra_random_positions = (synthesis_specs.size() > 1 ? ExtractInteger(synthesis_specs[1], 0) : 0);

        const vector<int> values = ExtractIntegers(synthesis_specs[0]);

        if(values.size() > 0)
        {
            vector<TemplatePtr> coherence_templars;
            for(unsigned int i = 0; i < values.size(); i++)
            {
                const int coherence_half_size = values[i];
                const vector<int> template_values(1, coherence_half_size);

                coherence_templars.push_back(BuildTemplate(neighborhood_specs[0], template_values, source.Dimension()));
            }
 
            const shared_ptr<Domain> coherence_domain = BuildDomain(output_boundary);

            const shared_ptr<Neighborhood> coherence_neighborhood = (coherence_templars.size() > 1 ? BuildNeighborhood(coherence_templars, *coherence_domain, *target_pyramid, pyramid_domain) : BuildNeighborhood(*coherence_templars[0], *coherence_domain));

            synthesizer.reset(new CoherenceSynthesizer(source, *input_neighborhood, *output_neighborhood, *coherence_neighborhood, penalty_range, zero_range, extra_random_positions));

            for(int k = 0; k < num_iterations; k++)
            {
                const string message = sequencer->Synthesize(*synthesizer, target);

                if(message != "")
                {
                    return message;
                }
            }
        }
        else
        {
            return "not enough values for coherence synthesis";
        }
    }
    else
    {
        return "unknown synthesis mode";
    }

    return "";
}

string Utility::ChooseSpec(const vector<string> & specs, const int index)
{
    return (specs.size() > 1 ? specs[index] : specs[0]);
}

bool Utility::LegalPyramidSpecSize(const int spec_size, const int pyramid_height)
{
    return ((spec_size == 1) || (spec_size == pyramid_height));
}
