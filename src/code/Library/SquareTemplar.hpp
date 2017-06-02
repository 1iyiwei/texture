/*
  SquareTemplar.hpp

  Li-Yi Wei
  August 21, 2014

*/

#ifndef _SQUARE_TEMPLAR_HPP
#define _SQUARE_TEMPLAR_HPP

#include "Templar.hpp"

class SquareTemplar : public Templar
{
public:
    SquareTemplar(const int dimension, const int half_size);
    virtual ~SquareTemplar(void);
};
#endif
