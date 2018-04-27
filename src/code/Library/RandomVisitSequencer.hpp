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
    RandomVisitSequencer(void);
    virtual bool Reset(const Texture & target);
    virtual bool Next(Position & answer);

private:
    vector<int> _target_size;
    int _total_count;
};
#endif
