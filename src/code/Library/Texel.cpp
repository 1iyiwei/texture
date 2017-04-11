/*
  Texel.cpp

  Li-Yi Wei
  August 17, 2014

*/

#include "Texel.hpp"


Texel::Texel(void) : _status(FREE), _range(0)
{
    // nothing else to do
}

Texel::Texel(const RangePtr & range, const Position & position) : _status(FREE), _range(range), _position(position)
{
    // nothing else to do
}

Texel::~Texel(void)
{
    // nothing else to do
    // range belongs to others
}

const Texel::Status Texel::GetStatus(void) const
{
    return _status;
}

bool Texel::SetStatus(const Status status)
{
    _status = status;
    return true;
}

const RangePtr Texel::GetRange(void) const
{
    return _range;
}

bool Texel::SetRange(const RangePtr & range)
{
    _range = range;

    return (_range != 0);
}

const Position & Texel::GetPosition(void) const
{
    return _position;
}

bool Texel::SetPosition(const Position & rhs)
{
    return true;
}

