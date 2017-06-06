/*
  Domain.hpp

  base class for all domains

  Li-Yi Wei
  October 16 2014
  (factor out from Neighborhood)

*/

#ifndef _DOMAIN_HPP
#define _DOMAIN_HPP

#include "Texture.hpp"

class Domain
{
public:
    virtual ~Domain(void) = 0;

    // handle various boundary condition by child classes
    virtual void Correct(const Texture & source, Position & position) const;

    // find the tiling position so that it is as near to anchor as possible
    // assume anchor is inside source
    virtual void Nearest(const Texture & source, const Position & anchor, Position & position) const;

    // L2 domain distance between anchor and position
    virtual int Geodesic2(const Texture & source, const Position & anchor, const Position & position) const;

};

#endif
