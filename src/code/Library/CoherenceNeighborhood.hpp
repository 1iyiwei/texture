/*
  CoherenceNeighborhood.hpp

  collect coherent neighbors

  Li-Yi Wei
  June 8, 2017

*/

#ifndef _COHERENCE_HEIGHBORHOOD_HPP
#define _COHERENCE_HEIGHBORHOOD_HPP

#include "Neighborhood.hpp"
#include "PyramidNeighborhood.hpp"

class CoherenceNeighborhood : public Neighborhood
{
public:
    CoherenceNeighborhood(const Texture & source, const Neighborhood & input_neighborhood, const Neighborhood & output_neighborhood, const Neighborhood & coherence_neighborhood);
    
    CoherenceNeighborhood(const Texture & source, const PyramidNeighborhood & input_neighborhood, const PyramidNeighborhood & output_neighborhood, const PyramidNeighborhood & coherence_neighborhood);
    
    virtual ~CoherenceNeighborhood(void);

    virtual vector<Neighbor> Neighbors(const Texture & texture, const Position & query) const;

    virtual vector<Position> Candidates(const Texture & texture, const Position & query) const;

protected:
    set<Position> & PositionSet(const vector<Position> & input, set<Position> & output) const;

protected:
    const Texture & _source_texture;

    const Neighborhood & _input_neighborhood;
    const Neighborhood & _output_neighborhood;
    const Neighborhood & _coherence_neighborhood;

    const PyramidNeighborhood * _input_pyramid_neighborhood_ptr;
    const PyramidNeighborhood * _output_pyramid_neighborhood_ptr;
    const PyramidNeighborhood * _coherence_pyramid_neighborhood_ptr;
};
#endif
