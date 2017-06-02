/*
  PyramidNeighborhood.hpp

  collecting neighborhood from texture + auxiliary pyramid

  can be used for multi-resolution and constrained synthesis
  keep the single resolution API instead of forcing multi-res due to latter

  Li-Yi Wei
  October 16, 2014

*/

#ifndef _PYRAMID_HEIGHBORHOOD_HPP
#define _PYRAMID_HEIGHBORHOOD_HPP

#include <vector>
using namespace std;

#include "Neighborhood.hpp"
#include "TexturePyramid.hpp"
#include "PyramidDomain.hpp"

class PyramidNeighborhood : public Neighborhood
{
public:
    // one templar per pyramid level and source
    // so (pyramid.Size()+1) == templars.size()
    PyramidNeighborhood(const TexturePyramid & pyramid, const PyramidDomain & pyramid_domain, const vector<TemplarPtr> & templars, const Domain & domain);
    virtual ~PyramidNeighborhood(void);

    const TexturePyramid & GetPyramid(void) const;
    const PyramidDomain & GetPyramidDomain(void) const;

    virtual vector<Neighbor> Neighbors(const Texture & source, const Position & position) const;

protected:
    TexturePyramid _pyramid;
    const PyramidDomain & _pyramid_domain;
    const vector<TemplarPtr> _templars;
};
#endif
