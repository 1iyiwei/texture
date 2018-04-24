/*
  Seamster.cpp

  Li-Yi Wei
  April 23, 2018

*/

#include "Seamster.hpp"
#include "Exception.hpp"

Seamster::Seamster(const Texture & source, const Domain & source_domain, const Neighborhood & target_neighborhood): _source(source), _source_domain(source_domain), _target_neighborhood(target_neighborhood)
{
    // nothing else to do
}

Seamster::~Seamster(void)
{
    // nothing else to do
}

bool Seamster::AtSeam(const Position & target_position, const Texture & target) const
{
    TexelPtr target_texel = 0;
    if(! target.Get(target_position, target_texel))
    {
        throw Exception("Seamster::AtSeam(): cannot get target");
    }

    if(!target_texel)
    {
        throw Exception("Seamster::AtSeam(): null target texel");
        // not yet synthesized texel is on the seam
        return true;
    }

    const Position & target_source_position = target_texel->GetPosition();

    // const Domain & target_domain = target_neighborhood.GetDomain();

    _target_neighbors = _target_neighborhood.Neighbors(target, target_position);

    bool at_seam = false; // answer to return

    for(unsigned int k = 0; !at_seam && (k < _target_neighbors.size()); k++)
    {
        const Neighborhood::Neighbor & neighbor = _target_neighbors[k];

        if((!neighbor.texel) || (neighbor.level >= 0))
        {
            // no need to pursue neighbor not containin texel or in different pyramid level
            continue;
        }

        if(neighbor.offset.size() != target_source_position.size())
        {
            throw Exception("Seamster::AtSeam(): offset size mismatch");
        }

        _neighbor_source_position = neighbor.texel->GetPosition();
        // match the fact that neighbor.offset contains original/uncorrected diffs
        _source_domain.Nearest(_source, target_source_position, _neighbor_source_position);

        if(_neighbor_source_position.size() != target_source_position.size())
        {
            throw Exception("Seamster::AtSeam(): position size mismatch");
        }

        for(unsigned int m = 0; !at_seam && (m < neighbor.offset.size()); m++)
        {
            if(neighbor.offset[m] != (_neighbor_source_position[m] - target_source_position[m]))
            {
                at_seam = true;
            }
        }
    }

    // done
    return at_seam;
}
