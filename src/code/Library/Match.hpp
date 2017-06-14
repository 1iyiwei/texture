/*
  Match.hpp

  measure neighborhood similarity

  Li-Yi Wei
  June 14 2017

*/

#ifndef _MATCH_HPP
#define _MATCH_HPP

#include "Neighborhood.hpp"

class Match
{
public:
    typedef Range::DistType DistType;

    struct Context
    {
        Context(const Texture & source, const Position & query, const Neighborhood & surround) : texture(source), position(query), neighborhood(surround) {};

        const Texture & texture;
        const Position & position;
        const Neighborhood & neighborhood;
    };

public:
    Match(const RangePtr & penalty_range, const RangePtr & zero_range);
    virtual ~Match(void);

    DistType Distance2(const Context & source, const Context & target) const;
    DistType Distance2(const Texture & source_texture, const Position & source_position, const Neighborhood & source_neighborhood, const Texture & target_texture, const Position & target_position, const Neighborhood & target_neighborhood) const;

protected:
    typedef Templar::Weight Weight;

    DistType Distance2(const vector<Neighborhood::Neighbor> & foo, const vector<Neighborhood::Neighbor> & bar) const;

    bool Penalize(vector<Neighborhood::Neighbor> & source, vector<Neighborhood::Neighbor> & target) const;

protected:
    const RangePtr _penalty_range;
    const RangePtr _zero_range;
};

#endif
