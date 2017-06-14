/*
  Match.cpp

  Li-Yi Wei
  June 14 2017

*/

#include "Match.hpp"
#include "Exception.hpp"

Match::Match(const RangePtr & penalty_range, const RangePtr & zero_range) : _penalty_range(penalty_range), _zero_range(zero_range)
{
    // nothing else to do
}

Match::~Match(void)
{
    // nothing else to do
}

Match::DistType Match::Distance2(const Context & source, const Context & target) const
{
    return Distance2(source.texture, source.position, source.neighborhood, target.texture, target.position, target.neighborhood);
}

Match::DistType Match::Distance2(const Texture & source_texture, const Position & source_position, const Neighborhood & source_neighborhood, const Texture & target_texture, const Position & target_position, const Neighborhood & target_neighborhood) const
{
    // target neighbors
    vector<Neighborhood::Neighbor> target_neighbors = target_neighborhood.Neighbors(target_texture, target_position);

    vector<Neighborhood::Neighbor> source_neighbors = source_neighborhood.Neighbors(source_texture, source_position);

    if(! Penalize(source_neighbors, target_neighbors))
    {
        throw Exception("Match::Distance2(): cannot penalize");
    }

    return Distance2(source_neighbors, target_neighbors);
}

Match::DistType Match::Distance2(const vector<Neighborhood::Neighbor> & foo, const vector<Neighborhood::Neighbor> & bar) const
{
    if(foo.size() != bar.size())
    {
        throw Exception("Match::Distance2(): size mismatch");
        return -1.0;
    }

    DistType total_answer = 0.0;
    Weight total_weight = 0.0;

    for(unsigned int k = 0; k < foo.size(); k++)
    {
        const ConstTexelPtr foo_texel(foo[k].texel);
        const ConstTexelPtr bar_texel(bar[k].texel);

        const Weight weight = foo[k].weight;

        if(foo_texel && bar_texel)
        {
            const RangePtr foo_range(foo_texel->GetRange());
            const RangePtr bar_range(bar_texel->GetRange());

            if(foo_range && bar_range)
            {
                const DistType answer = foo_range->Distance2(*bar_range);

                if(answer < 0) throw Exception("Match::Distance2(): negative dist2");

                total_answer += weight*answer;
                total_weight += weight;
            }
            else
            {
                throw Exception("Match::Distance2(): null range");
            }
        }
    }

    // normalization
    if(total_weight > 0) 
    {
        total_answer /= total_weight;
    }
		
    // done
    return total_answer;
}

bool Match::Penalize(vector<Neighborhood::Neighbor> & source, vector<Neighborhood::Neighbor> & target) const
{
    if(source.size() != target.size())
    {
        return false;
    }

    for(unsigned int k = 0; k < source.size(); k++)
    {
        if(!source[k].texel && target[k].texel)
        {
            // arbitrary position for source
            source[k].texel.reset(new Texel(_penalty_range, target[k].texel->GetPosition()));
            
            target[k].texel.reset(new Texel(_zero_range, target[k].texel->GetPosition()));
        }
    }

    return true;
}
