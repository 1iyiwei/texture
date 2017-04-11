/*
  Neighborhood.cpp

  Li-Yi Wei
  August 21, 2014

*/

#include <string>
using namespace std;

#include "Neighborhood.hpp"
#include "Exception.hpp"

Neighborhood::Neighborhood(const Domain & domain) : _domain(domain)
{
    // nothing else to do
}

Neighborhood::~Neighborhood(void)
{
    // nothing else to do
}

const Domain & Neighborhood::GetDomain(void) const
{
    return _domain;
}

vector<Neighborhood::Neighbor> Neighborhood::Neighbors(const Texture & source, const Position & position) const
{
    throw Exception("Neighborhood::Neighbors(): shouldn't be called");

    vector<Neighborhood::Neighbor> foo;

    return foo;
}
