/*
  ToroidalDomain.hpp

  assumes null beyond texture boundary

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _TOROIDAL_DOMAIN_HPP
#define _TOROIDAL_DOMAIN_HPP

#include "Domain.hpp"

class ToroidalDomain : public Domain
{
public:
    ToroidalDomain(void);
    virtual ~ToroidalDomain(void);

    virtual void Correct(const Texture & source, Position & position) const;

    virtual void Nearest(const Texture & source, const Position & anchor, Position & position) const;

    virtual int Geodesic2(const Texture & source, const Position & anchor, const Position & position) const;

protected:
    static int Correct(const int source, const int position);

    static int Nearest(const int source, const int anchor, const int position);

    static int Geodesic(const int source, const int anchor, const int position);
};

#endif
