/*
  Seamster.hpp

  decides if a texel is on seam or not

  Li-Yi Wei
  April 23, 2018

*/

#ifndef _SEAMSTER_HPP
#define _SEAMSTER_HPP

#include "Texture.hpp"
#include "Neighborhood.hpp"

class Seamster
{
public:
    Seamster(const Texture & source, const Domain & source_domain, const Neighborhood & target_neighborhood);
    virtual ~Seamster(void);

    // decide whether a target position is at seam
    virtual bool AtSeam(const Position & target_position, const Texture & target) const;

private:
    const Texture & _source;
    const Domain & _source_domain;
    const Neighborhood & _target_neighborhood;

    mutable Position _neighbor_source_position;
    mutable vector<Neighborhood::Neighbor> _target_neighbors;
};

#endif
