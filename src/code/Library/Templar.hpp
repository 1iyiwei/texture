/*
  Templar.hpp

  the base class for all neighborhood shape templates

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _TEMPLAR_HPP
#define _TEMPLAR_HPP

#include <vector>
#include <memory>
using namespace std;

#include "Texel.hpp"

class Templar
{
public:

    typedef float Weight;

    virtual ~Templar(void) = 0;

    virtual int Size(void) const;
    virtual const Position & GetOffset(const int index) const;
    virtual Weight GetWeight(const int index) const;

    virtual const vector<Position> & GetOffsets(void) const;
    virtual const vector<Weight> & GetWeights(void) const;

protected:
    vector<Position> _offsets;
    vector<Weight> _weights;
};

typedef shared_ptr<Templar> TemplarPtr;
#endif
