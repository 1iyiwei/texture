/*
  Synthesizer.cpp

  Li-Yi Wei
  August 17, 2014

*/

#include "Synthesizer.hpp"
#include "Exception.hpp"

Synthesizer::~Synthesizer(void)
{
    // nothing else to do
}

string Synthesizer::Synthesize(const Position & position, Texture & target) const
{
    // should not be called
    return "Synthesizer::Synthesize() shouldn't be called";
}
