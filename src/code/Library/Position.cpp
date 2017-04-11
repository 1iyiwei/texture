/*
  Position.cpp

  Li-Yi Wei
  August 22, 2014

*/

#include "Position.hpp"

ostream & operator<<(ostream & out, const Position & position)
{
    out << "[";
    for(unsigned int k = 0; k < position.size(); k++)
    {
        out << position[k];
        if((k+1) < position.size()) out << ", ";
    }
    out << "]";

    return out;
}
