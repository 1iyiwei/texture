/*
  Templar.cpp

  Li-Yi Wei
  August 21, 2014

*/

#include "Templar.hpp"
#include "Exception.hpp"

Templar::~Templar(void)
{
    // nothing else to do
}

int Templar::Size(void) const
{
    return _offsets.size();
}

const Position & Templar::GetOffset(const int index) const
{
    if((index < 0) || (static_cast<unsigned int>(index) >= _offsets.size())) throw Exception("Templar::GetOffset(): out of bound");

    return _offsets[index];
}

Templar::Weight Templar::GetWeight(const int index) const
{
    return 1.0;
}

const vector<Position> & Templar::GetOffsets(void) const
{
    return _offsets;
}

const vector<Templar::Weight> & Templar::GetWeights(void) const
{
    return _weights;
}
