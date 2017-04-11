/*
  SquareTemplate.cpp

  Li-Yi Wei
  August 21, 2014

*/

#include "SquareTemplate.hpp"
#include "SequentialCounter.hpp"
#include "Exception.hpp"

SquareTemplate::SquareTemplate(const int dimension, const int half_size)
{
    SequentialCounter counter(dimension, -half_size, half_size);

    _offsets.clear();
    _weights.clear();

    counter.Reset();
    Position position;
    
    do
    {
        counter.Get(position);
        _offsets.push_back(position);
    }
    while(counter.Next());

    _weights = vector<Weight>(_offsets.size(), 1.0);
}

SquareTemplate::~SquareTemplate(void)
{
    // nothing else to do
}
