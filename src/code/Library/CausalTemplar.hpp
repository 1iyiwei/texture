/*
  CausalTemplar.hpp

  convert any other template into causal shape

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _CAUSAL_TEMPLAR_HPP
#define _CAUSAL_TEMPLAR_HPP

#include "Templar.hpp"

class CausalTemplar : public Templar
{
public:
    CausalTemplar(const Templar & source);
    virtual ~CausalTemplar(void);

protected:
    static bool IsCausal(const Position & position);
};
#endif

  
  
