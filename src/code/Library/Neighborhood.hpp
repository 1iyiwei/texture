/*
  Neighborhood.hpp

  the base class for all neighborhoods

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _NEIGHBORHOOD_HPP
#define _NEIGHBORHOOD_HPP

#include "Templar.hpp"
#include "Domain.hpp"
#include "Range.hpp"
#include "Texture.hpp"

// #define _RESTRICT_ACCESS

class Neighborhood
{
public:
#ifdef _RESTRICT_ACCESS
    friend class Match;
#endif

    typedef Templar::Weight Weight;

    struct Neighbor
    {
        Neighbor(void) : level(-1), weight(1) {};

        ConstTexelPtr texel;
        Position offset;
        int level; // for the auxiliary pyramid, if any
        Weight weight;
    };

public:
    Neighborhood(const Domain & domain);
    virtual ~Neighborhood(void) = 0;

    const Domain & GetDomain(void) const;

#ifdef _RESTRICT_ACCESS
protected:
    friend class CoherenceNeighborhood;
#endif

    virtual vector<Neighbor> Neighbors(const Texture & source, const Position & position) const = 0;

protected:
    const Domain & _domain;
};

#endif
