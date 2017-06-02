/*
  PyramidNeighborhood.cpp

  Li-Yi Wei
  October 17, 2014

*/

#include <algorithm>
using namespace std;

#include "PyramidNeighborhood.hpp"
#include "PlainNeighborhood.hpp"
#include "Exception.hpp"

PyramidNeighborhood::PyramidNeighborhood(const TexturePyramid & pyramid, const PyramidDomain & pyramid_domain, const vector<TemplarPtr> & templars, const Domain & domain) : Neighborhood(domain), _pyramid(pyramid), _pyramid_domain(pyramid_domain), _templars(templars)
{
#if 0 // no need
    if(static_cast<unsigned int>(_pyramid.Size() + 1) != _templars.size())
    {
        throw Exception("PyramidNeighborhood::PyramidNeighborhood(): incompatible pyramid and templars");
    }
#endif
}

PyramidNeighborhood::~PyramidNeighborhood(void)
{
    // nothing else to do
}

const TexturePyramid & PyramidNeighborhood::GetPyramid(void) const
{
    return _pyramid;
}

const PyramidDomain & PyramidNeighborhood::GetPyramidDomain(void) const
{
    return _pyramid_domain;
}

vector<Neighborhood::Neighbor> PyramidNeighborhood::Neighbors(const Texture & source, const Position & position) const
{
    const vector<int> source_size = source.Size();

    vector<Neighborhood::Neighbor> neighbors;

    for(int level = 0; level < min(static_cast<int>(_templars.size()), _pyramid.Size()+1); level++)
    {
        const int pyramid_level = level-1;
        const Texture & level_texture = (level == 0 ? source : *_pyramid.Get(pyramid_level));
 
        Position level_position;

        if(! _pyramid_domain.Trace(source_size, position, level_texture.Size(), level_position))
        {
            throw Exception("PyramidNeighborhood::Neighbors(): cannot trace");
        }

        PlainNeighborhood level_neighborhood(*_templars[level], _domain);
         
        vector<Neighborhood::Neighbor> level_neighbors = level_neighborhood.Neighbors(level_texture, level_position);
 
        for(unsigned int i = 0; i < level_neighbors.size(); i++)
        {
            level_neighbors[i].level = pyramid_level;
        }

        neighbors.insert(neighbors.end(), level_neighbors.begin(), level_neighbors.end());
    }

    return neighbors;
}
