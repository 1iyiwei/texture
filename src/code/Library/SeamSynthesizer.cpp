/*
  SeamSynthesizer.cpp

  Li-Yi Wei
  April 24, 2018

*/

#include "SeamSynthesizer.hpp"

SeamSynthesizer::SeamSynthesizer(const Synthesizer & synthesizer, const Seamster & seamster): _synthesizer(synthesizer), _seamster(seamster)
{
    // nothing else to do
}

SeamSynthesizer::~SeamSynthesizer(void)
{
    // nothing else to do
}

string SeamSynthesizer::Synthesize(const Position & position, Texture & target) const
{
    if(_seamster.AtSeam(position, target))
    {
        return _synthesizer.Synthesize(position, target);
    }
    else
    {
        return "";
    }
}
