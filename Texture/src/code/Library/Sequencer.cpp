/*
  Sequencer.cpp

  Li-Yi Wei
  August 17, 2014

*/

#include "Sequencer.hpp"

Sequencer::~Sequencer(void)
{
    // nothing else to do
}

string Sequencer::Synthesize(const Synthesizer & synthesizer, Texture & target) const
{
    // should not be called
    return "Sequencer::Synthesize(): shouldn't be called";
}
