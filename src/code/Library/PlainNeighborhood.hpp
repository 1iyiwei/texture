/*
  PlainNeighborhood.hpp

  the vanilla version, collecting neighbors from a single image

  Li-Yi Wei
  October 16, 2014

*/

#ifndef _PLAIN_HEIGHBORHOOD_HPP
#define _PLAIN_HEIGHBORHOOD_HPP

#include "Neighborhood.hpp"

class PlainNeighborhood : public Neighborhood
{
public:

    PlainNeighborhood(const Templar & templar, const Domain & domain);
    virtual ~PlainNeighborhood(void);

    virtual vector<Neighbor> Neighbors(const Texture & source, const Position & position) const;

protected:
    const Templar & _templar;
};
#endif
