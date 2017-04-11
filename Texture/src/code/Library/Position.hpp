/*
  Position.hpp

  texel positions

  Li-Yi Wei
  August 22, 2014

*/

#ifndef _POSITION_HPP
#define _POSITION_HPP

#include <ostream>
#include <vector>
using namespace std;

typedef vector<int> Position;

ostream & operator<<(ostream & out, const Position & position);

#endif
