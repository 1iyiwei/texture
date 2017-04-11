/*
  Synthesizer.cpp

  Li-Yi Wei
  August 17, 2014

*/

#include "Synthesizer.hpp"
#include "Exception.hpp"

Synthesizer::~Synthesizer(void)
{
    // nothing else to do
}

string Synthesizer::Synthesize(const Position & position, Texture & target) const
{
    // should not be called
    return "Synthesizer::Synthesize() shouldn't be called";
}
 
Synthesizer::DistType Synthesizer::Distance2(const vector<Neighborhood::Neighbor> & foo, const vector<Neighborhood::Neighbor> & bar) const
{
    if(foo.size() != bar.size())
    {
        throw Exception("Synthesizer::Distance2(): size mismatch");
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

                if(answer < 0) throw Exception("Synthesizer::Distance2(): negative dist2");

                total_answer += weight*answer;
                total_weight += weight;
            }
            else
            {
                throw Exception("Synthesizer::Distance2(): null range");
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

set<Position> & Synthesizer::PositionSet(const vector<Position> & input, set<Position> & output) const
{
    output.clear();

    for(unsigned int k = 0; k < input.size(); k++)
    {
        output.insert(input[k]);
    }

    return output;
}
