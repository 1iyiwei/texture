/*
  Utility.hpp

  a collection of convenience routines

  Li-Yi Wei
  August 17, 2014

*/

#ifndef _UTILITY_HPP
#define _UTILITY_HPP

#include <vector>
#include <string>
#include <memory>
using namespace std;

#include "Template.hpp"
#include "Neighborhood.hpp"
#include "Sequencer.hpp"
#include "TexturePyramid.hpp"

class Utility
{
public:
    static vector<int> Minus1(const vector<int> & input);

    static vector<string> & Split(const string & input, char delim, vector<string> & output); 

    static vector<string> Split(const string & input, char delim);

    static vector<int> ExtractIntegers(const string & input);
    static int ExtractInteger(const string & input, const int default_value);

    static string ReadPyramid(const string & level_file_names, TexturePyramidPtr & output, int & maximum_value);

    static shared_ptr<Template> BuildTemplate(const string & neighborhood_spec, const int dimension);
    static shared_ptr<Template> BuildTemplate(const string & neighborhood_spec, const vector<int> & values, const int dimension);
 
    static shared_ptr<Domain> BuildDomain(const string & boundary_condition);

    static shared_ptr<Neighborhood> BuildNeighborhood(const Template & templar, const Domain & domain);
    static shared_ptr<Neighborhood> BuildNeighborhood(const vector<TemplatePtr> & templars, const Domain & domain, const TexturePyramid & pyramid, const PyramidDomain & pyramid_domain);

    static shared_ptr<Sequencer> BuildSequencer(const string & sequence_spec, const vector<Position> & diffusion_seeds, const Neighborhood & neighborhood);

    /* return "" if OK, error message otherwise */

    static string ReadConstraints(const string & input_file_name, const int domain_dimension, const int range_dimension, vector<TexelPtr> & constraints);

    static TexelPtr SatisfyConstraint(const TexelPtr & constraint, const vector<TexelPtr> & candidates);

    // the position of each texel specifies the target not source position
    static string HardConstrain(const vector<TexelPtr> & constraints, const Texture & source, Texture & target);
    static string HardConstrain(const vector<TexelPtr> & constraints, const PyramidDomain & pyramid_domain, const TexturePyramid & source_pyramid, TexturePyramid & target_pyramid);

    // get hard constraint positions
    static string GetConstraints(const Texture & texture, vector<Position> & positions);
    static string GetConstraints(const TexturePyramid & texture_pyramid, vector< vector<Position> > & positions);

#if 0
    static string Initialize(const string & input_boundary, const string & output_boundary, const string & neighborhood_spec, const Texture & source, Texture & target);
#endif

    // spec separator chars:
    // PYRAMID for computing each pyramid level
    // LEVEL for specifying multi-resolution within each level
    // PASS for multiple passes within a level
    
    typedef enum {PASS, LEVEL, PYRAMID} SeparatorType;
    static char Separator(const SeparatorType type);

    // per pyramid synthesis
    static string Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const TexturePyramid & source, TexturePyramid & target, const RangePtr & penalty_range, const RangePtr & zero_range);

    static string Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const TexturePyramid & source, TexturePyramid & target, const RangePtr & penalty_range, const RangePtr & zero_range, const vector< vector<Position> > & diffusion_seeds);

    // per level synthesis
    static string Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, Texture & target, const RangePtr & penalty_range, const RangePtr & zero_range);
    
    static string Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, Texture & target, const RangePtr & penalty_range, const RangePtr & zero_range, const vector<Position> & diffusion_seeds);

    static string Synthesize(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, const TexturePyramidPtr & source_pyramid, Texture & target, const TexturePyramidPtr & target_pyramid, const RangePtr & penalty_range, const RangePtr & zero_range, const vector<Position> & diffusion_seeds);

    // per pass synthesis
    static string SynthesizeOnce(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, Texture & target, const RangePtr & penalty_range, const RangePtr & zero_range, const vector<Position> & diffusion_seeds);

    static string SynthesizeOnce(const string & input_boundary, const string & output_boundary, const string & synthesis_spec, const string & sequence_spec, const string & neighborhood_spec, const Texture & source, const TexturePyramidPtr & source_pyramid, Texture & target, const TexturePyramidPtr & target_pyramid, const RangePtr & penalty_range, const RangePtr & zero_range, const vector<Position> & diffusion_seeds);

protected:
    static string ChooseSpec(const vector<string> & specs, const int index);
    static bool LegalPyramidSpecSize(const int spec_size, const int pyramid_height);

protected:
    // these need to be consistent with the scripts
    static const char _UNKNOWN_SEPARATOR = '?';
    static const char _PASS_SEPARATOR = '+';
    static const char _LEVEL_SEPARATOR = '*';
    static const char _PYRAMID_SEPARATOR = '-';
    static const char _PARAM_GROUP_SEPARATOR = '.';
};
#endif
