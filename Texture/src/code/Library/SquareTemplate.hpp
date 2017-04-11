/*
  SquareTemplate.hpp

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _SQUARE_TEMPLATE_HPP
#define _SQUARE_TEMPLATE_HPP

#include "Template.hpp"

class SquareTemplate : public Template
{
public:
    SquareTemplate(const int dimension, const int half_size);
    virtual ~SquareTemplate(void);
};
#endif
