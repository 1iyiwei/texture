/*
  PyramidDomain.cpp

  Li-Yi Wei
  October 17, 2014

*/

#include "PyramidDomain.hpp"

PyramidDomain::~PyramidDomain(void)
{
    // nothing else to do
}

bool PyramidDomain::Trace(const vector<int> & source_size, const vector<int> & source_index, const vector<int> & destination_size, vector<int> & destination_index) const
{
    // shouldn't be called
    return false;
}
