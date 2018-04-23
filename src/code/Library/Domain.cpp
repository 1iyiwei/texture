/*
  Domain.cpp

  Li-Yi Wei
  October 16, 2014

*/

#include "Domain.hpp"
#include "Exception.hpp"

Domain::~Domain(void)
{
    // nothing else to do
}

void Domain::Correct(const Texture & source, Position & position) const
{
    // do nothing by default
}

void Domain::Nearest(const Texture & source, const Position & anchor, Position & position) const
{
    // do nothing by default
}

int Domain::Geodesic2(const Texture & source, const Position & anchor, const Position & position) const
{
    Position altered = position;
    Nearest(source, anchor, altered);

    if(anchor.size() != altered.size())
    {
        throw Exception("Domain::Geodesic2(): dimensionality mismatch");
    }

    int answer = 0;
    for(unsigned int i = 0; i < anchor.size(); i++)
    {
        const int diff = anchor[i]-altered[i];
        answer += diff*diff;
    }

    return answer;
}

bool Domain::SameDiff(const Texture & source, const Position & source_diff, const Position & target_diff) const
{
    if(source_diff.size() != target_diff.size())
    {
        return false;
    }

    bool answer = true;

    for(unsigned int k = 0; k < target_diff.size(); k++)
    {
        if(source_diff[k] != target_diff[k])
        {
            answer = false;
        }
    }

    return answer;
}
