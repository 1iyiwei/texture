/*
  Template.cpp

  Li-Yi Wei
  August 21, 2014

*/

#include "Template.hpp"
#include "Exception.hpp"

Template::~Template(void)
{
    // nothing else to do
}

int Template::Size(void) const
{
    return _offsets.size();
}

const Position & Template::GetOffset(const int index) const
{
    if((index < 0) || (static_cast<unsigned int>(index) >= _offsets.size())) throw Exception("Template::GetOffset(): out of bound");

    return _offsets[index];
}

Template::Weight Template::GetWeight(const int index) const
{
    return 1.0;
}

const vector<Position> & Template::GetOffsets(void) const
{
    return _offsets;
}

const vector<Template::Weight> & Template::GetWeights(void) const
{
    return _weights;
}
