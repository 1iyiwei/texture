/*
  PlainNeighborhood.cpp

  Li-Yi Wei
  October 16, 2014

*/

#include "PlainNeighborhood.hpp"
#include "Exception.hpp"

PlainNeighborhood::PlainNeighborhood(const Templar & templar, const Domain & domain) : Neighborhood(domain), _templar(templar)
{
    // nothing else to do
}

PlainNeighborhood::~PlainNeighborhood(void)
{
    // nothing else to do
}

vector<Neighborhood::Neighbor> PlainNeighborhood::Neighbors(const Texture & source, const Position & position) const
{
    vector<Neighborhood::Neighbor> neighbors;

    Neighbor neighbor;
    Position corrected;
    for(int i = 0; i < _templar.Size(); i++)
    {
        neighbor.offset = _templar.GetOffset(i);
        neighbor.weight = _templar.GetWeight(i);
        
        corrected = position;
        if(corrected.size() != neighbor.offset.size())
        {
            throw Exception("Neighborhood::Neighbors(): corrected.size() " + to_string(static_cast<unsigned long long>(corrected.size())) + " != neighbor.offset.size() " + to_string(static_cast<unsigned long long>(neighbor.offset.size())));
        }

        for(unsigned int k = 0; k < corrected.size(); k++)
        {
            corrected[k] += neighbor.offset[k];
        }

        _domain.Correct(source, corrected);
        
        TexelPtr texel;
        if(! source.Get(corrected, texel))
        {
            texel = 0;
        }
        neighbor.texel = texel;

        neighbors.push_back(neighbor);
    }

    return neighbors;
}
