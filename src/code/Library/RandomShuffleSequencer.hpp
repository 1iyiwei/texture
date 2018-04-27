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
    RandomShuffleSequencer(void);
    virtual bool Reset(const Texture & target);
    virtual bool Next(Position & answer);

private:
    vector<Position> _positions;
    unsigned int _current_position; 
};
#endif
