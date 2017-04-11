/*
  Range.cpp

  Li-Yi Wei
  August 16, 2014

*/

#include "Range.hpp"
#include "Exception.hpp"

Range::~Range(void)
{
    // nothing else to do
}

Range::DistType Range::Distance2(const Range & rhs) const
{
    throw Exception(" Range::Distance2(): shouldn't be called");
    return 0;
}
