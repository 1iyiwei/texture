/*
  PlainDomain.cpp

  Li-Yi Wei
  August 21, 2014

*/

#include "PlainDomain.hpp"
#include "Exception.hpp"

PlainDomain::PlainDomain(void)
{
    // nothing else to do
}

PlainDomain::~PlainDomain(void)
{
    // nothing else to do
}

void PlainDomain::Correct(const Texture & source, Position & position) const
{
    // keep the original position
}

void PlainDomain::Nearest(const Texture & source, const Position & anchor, Position & position) const
{
    // keep the original position
}
 
int PlainDomain::NearestDistance2(const Texture & source, const Position & anchor, const Position & position) const
{
    if(anchor.size() != position.size())
    {
        throw Exception("PlainDomain::NearestDistance2(): dimensionality mismatch");
    }

    int answer = 0;
    for(unsigned int i = 0; i < anchor.size(); i++)
    {
        const int diff = anchor[i]-position[i];
        answer += diff*diff;
    }

    return answer;
}
