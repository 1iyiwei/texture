/*
  Texel.hpp

  texture samples with discrete (integer grid) positions

  Li-Yi Wei
  August 17, 2014

*/

#ifndef _TEXEL_HPP
#define _TEXEL_HPP

#include <vector>
#include <memory>
using namespace std;

#include "Range.hpp"
#include "Position.hpp"

class Texel
{
public:
    enum Status {FREE, FIXED};

public:
    Texel(void);
    Texel(const RangePtr & range, const Position & position);
    ~Texel(void);

    const Status GetStatus(void) const;
    bool SetStatus(const Status status);

    // color, etc.
    const RangePtr GetRange(void) const;
    bool SetRange(const RangePtr & range);

    // the original source position
    const Position & GetPosition(void) const;
    bool SetPosition(const Position & position);

protected:
    Status _status;
    RangePtr _range;
    Position _position;
};

typedef shared_ptr<Texel> TexelPtr;
typedef shared_ptr<const Texel> ConstTexelPtr;

#endif
