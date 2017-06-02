/*
  Synthesizer.hpp

  the base class for all synthesizers

  Li-Yi Wei
  August 17, 2014

*/

#ifndef _SYNTHESIZER_HPP
#define _SYNTHESIZER_HPP

#include <set>
#include <string>
using namespace std;

#include "Texture.hpp"
#include "Neighborhood.hpp"

class Synthesizer
{
public:
    virtual ~Synthesizer(void) = 0;

    // synthesize a specific texel at target(position)
    virtual string Synthesize(const Position & position, Texture & target) const;

protected:
    typedef Range::DistType DistType;
    typedef Templar::Weight Weight;

    DistType Distance2(const vector<Neighborhood::Neighbor> & foo, const vector<Neighborhood::Neighbor> & bar) const;

    set<Position> & PositionSet(const vector<Position> & input, set<Position> & output) const;
};

#endif
