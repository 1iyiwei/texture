/*
  CausalTemplate.hpp

  convert any other template into causal shape

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _CAUSAL_TEMPLATE_HPP
#define _CAUSAL_TEMPLATE_HPP

#include "Template.hpp"

class CausalTemplate : public Template
{
public:
    CausalTemplate(const Template & source);
    virtual ~CausalTemplate(void);

protected:
    static bool IsCausal(const Position & position);
};
#endif

  
  
