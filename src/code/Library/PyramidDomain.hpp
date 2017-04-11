/*
  PyramidDomain.hpp
  
  base class for all pyramid domains

  Li-Yi Wei
  October 17, 2014

*/

#ifndef _PYRAMID_DOMAIN_HPP
#define _PYRAMID_DOMAIN_HPP

#include <vector>
#include <memory>
using namespace std;

class PyramidDomain
{
public:
    virtual ~PyramidDomain(void) = 0;

    // trace corresponding indices between levels like ancestry/offspring
    virtual bool Trace(const vector<int> & source_size, const vector<int> & source_index, const vector<int> & destination_size, vector<int> & destination_index) const = 0;
};

typedef shared_ptr<PyramidDomain> PyramidDomainPtr;

#endif
