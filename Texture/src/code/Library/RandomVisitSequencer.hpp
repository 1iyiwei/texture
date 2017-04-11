/*
  RandomVisitSequencer.hpp

  randomly visit pixels with no guarantee of uniqueness or duplication
  (contrast to RandomShuffleSequencer)

  Li-Yi Wei
  August 23, 2014

*/

#ifndef _RANDOM_VISIT_SEQUENCER_HPP
#define _RANDOM_VISIT_SEQUENCER_HPP

#include "Sequencer.hpp"

class RandomVisitSequencer : public Sequencer
{
public:
    virtual string Synthesize(const Synthesizer & synthesizer, Texture & target) const;
};
#endif
