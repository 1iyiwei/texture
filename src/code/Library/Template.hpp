/*
  Template.hpp

  the base class for all neighborhood shape templates

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _TEMPLATE_HPP
#define _TEMPLATE_HPP

#include <vector>
#include <memory>
using namespace std;

#include "Texel.hpp"

class Template
{
public:

    typedef float Weight;

    virtual ~Template(void) = 0;

    virtual int Size(void) const;
    virtual const Position & GetOffset(const int index) const;
    virtual Weight GetWeight(const int index) const;

    virtual const vector<Position> & GetOffsets(void) const;
    virtual const vector<Weight> & GetWeights(void) const;

protected:
    vector<Position> _offsets;
    vector<Weight> _weights;
};

typedef shared_ptr<Template> TemplatePtr;
#endif
