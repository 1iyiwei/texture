/*
  Sequencer.hpp

  the base class of all sequencers

  Li-Yi Wei
  August 17, 2014

*/

#ifndef _SEQUENCER_HPP
#define _SEQUENCER_HPP

#include <string>
using namespace std;

#include "Texture.hpp"

class Sequencer
{
public:
    virtual ~Sequencer(void) = 0;

    // reset the sequencer
    virtual bool Reset(const Texture & target);
    // next synthesis position; return whether valid or not
    virtual bool Next(Position & answer);
};
#endif
