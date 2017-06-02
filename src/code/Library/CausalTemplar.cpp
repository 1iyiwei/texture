/*
  CausalTemplar.cpp

  Li-Yi Wei
  August 21, 2014

*/

#include "CausalTemplar.hpp"


CausalTemplar::CausalTemplar(const Templar & source)
{
    Position offset;
    Weight weight;

    _offsets.clear();
    _weights.clear();

    for(int i = 0; i < source.Size(); i++)
    {
        offset = source.GetOffset(i);
        weight = source.GetWeight(i);

        if(IsCausal(offset))
        {
            _offsets.push_back(offset);
            _weights.push_back(weight);
        }
    }
}

CausalTemplar::~CausalTemplar(void)
{
    // nothing else to do
}

bool CausalTemplar::IsCausal(const Position & position)
{
    for(unsigned int i = 0; i < position.size(); i++)
    {
        const int value = position[i];

        if(value > 0)
            return false;
        else if(value < 0)
            return true;
        else
            continue;
    }

    // not including myself
    return false;
}
