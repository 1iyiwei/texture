/*
  Counter.cpp

  Li-Yi Wei
  07/07/2007

*/

#include "Counter.hpp"
#include <assert.h>

int Counter::Sortee::operator<(const Sortee & another) const
{
    return (key < another.key);
}


Counter::Counter(const int dimension) : _dimension(dimension)
{
    // nothing else to do
}

Counter::~Counter(void)
{
    // nothing to do
}

int Counter::Reset(void)
{
    // shouldn't be called
    assert(0);
    return 0;
}

int Counter::Get(vector<int> & value) const
{
    // shouldn't be called
    assert(0);
    return 0;
}
    
int Counter::Next(void)
{
    // shouldn't be called
    assert(0);
    return 0;
}

int Counter::Dimension(void) const
{
    return _dimension;
}
