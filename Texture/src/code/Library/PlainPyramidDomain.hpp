/*
  PlainPyramidDomain.hpp

  Li-Yi Wei
  October 17, 2014

*/

#ifndef _PLAIN_PYRAMID_DOMAIN_HPP
#define _PLAIN_PYRAMID_DOMAIN_HPP

#include "PyramidDomain.hpp"

class PlainPyramidDomain : public PyramidDomain
{
public:

    bool Trace(const vector<int> & source_size, const vector<int> & source_index, const vector<int> & destination_size, vector<int> & destination_index) const;
};
#endif
