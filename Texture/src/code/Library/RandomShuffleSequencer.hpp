/*
  RandomShuffleSequencer.hpp

  visit pixels in a random shuffled order

  Li-Yi Wei
  August 24, 2014

*/

#ifndef _RANDOM_SHUFFLE_SEQUENCER_HPP
#define _RANDOM_SHUFFLE_SEQUENCER_HPP

#include "Sequencer.hpp"

class RandomShuffleSequencer : public Sequencer
{
public:
    virtual string Synthesize(const Synthesizer & synthesizer, Texture & target) const;
};
#endif
