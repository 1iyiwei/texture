/*
  CoherenceSynthesizer.hpp

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _COHERENCE_SYNTHESIZER_HPP
#define _COHERENCE_SYNTHESIZER_HPP

#include "Synthesizer.hpp"
#include "CoherenceNeighborhood.hpp"

class CoherenceSynthesizer : public Synthesizer
{
public:
    CoherenceSynthesizer(const Texture & source, const Neighborhood & input_neighborhood, const Neighborhood & output_neighborhood, const Neighborhood & coherence_neighborhood, const RangePtr & penalty_range, const RangePtr & zero_range, const int num_extra_random_positions);

    CoherenceSynthesizer(const Texture & source, const PyramidNeighborhood & input_neighborhood, const PyramidNeighborhood & output_neighborhood, const PyramidNeighborhood & coherence_neighborhood, const RangePtr & penalty_range, const RangePtr & zero_range, const int num_extra_random_positions);

    virtual ~CoherenceSynthesizer(void);

    virtual string Synthesize(const Position & position, Texture & target) const;

protected:

    bool Penalize(vector<Neighborhood::Neighbor> & source, vector<Neighborhood::Neighbor> & target) const;

protected:
    const Texture & _source_texture;

    const Neighborhood & _input_neighborhood;
    const Neighborhood & _output_neighborhood;
    const CoherenceNeighborhood _coherence_neighborhood;

    const RangePtr _penalty_range;
    const RangePtr _zero_range;

    const int _num_extra_random_positions;
};

#endif
