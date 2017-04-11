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

#include "Synthesizer.hpp"
#include "Texture.hpp"

class Sequencer
{
public:
    virtual ~Sequencer(void) = 0;

    // synthesize the target in a certain order/fashion
    virtual string Synthesize(const Synthesizer & synthesizer, Texture & target) const;
};
#endif
