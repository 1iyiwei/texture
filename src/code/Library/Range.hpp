/*
  Range.hpp

  the base class for all payloads, such as texture color
  designed for better orthogonality with respect to the core synthesis

  This could be implemented via c++ template
  but opt for class now for better modularity

  Li-Yi Wei
  August 16, 2014

*/

#ifndef _RANGE_HPP
#define _RANGE_HPP

#include <memory>
using namespace std;
 
class Range
{
public:
    typedef float DistType;

    virtual ~Range(void) = 0;

    virtual DistType Distance2(const Range & rhs) const;
};

typedef shared_ptr<Range> RangePtr;

#endif
