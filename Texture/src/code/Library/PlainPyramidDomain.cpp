/*
  PlainPyramidDomain.cpp

  Li-Yi Wei
  October 17, 2014

*/

#include "PlainPyramidDomain.hpp"

bool PlainPyramidDomain::Trace(const vector<int> & source_size, const vector<int> & source_index, const vector<int> & destination_size, vector<int> & destination_index) const
{
    if((source_size.size() != source_index.size()) || (source_size.size() != destination_size.size()))
    {
        return false;
    }

    if(destination_index.size() != source_index.size())
    {
        destination_index = source_index;
    }

    for(unsigned int i = 0; i < destination_index.size(); i++)
    {
        destination_index[i] = source_index[i]*destination_size[i]/source_size[i];
    }

    return true;
}
