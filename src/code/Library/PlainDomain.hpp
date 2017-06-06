/*
  PlainDomain.hpp

  assumes null beyond texture boundary

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _PLAIN_DOMAIN_HPP
#define _PLAIN_DOMAIN_HPP

#include "Domain.hpp"

class PlainDomain : public Domain
{
public:
    PlainDomain(void);
    virtual ~PlainDomain(void);

    virtual void Correct(const Texture & source, Position & position) const;

    virtual void Nearest(const Texture & source, const Position & anchor, Position & position) const;

    virtual int Geodesic2(const Texture & source, const Position & anchor, const Position & position) const;
};

#endif
