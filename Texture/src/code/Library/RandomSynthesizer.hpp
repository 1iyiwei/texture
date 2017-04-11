/*
  RandomSynthesizer.hpp

  randomly copy from source to target; good for initialization

  Li-Yi Wei
  August 17, 2014

*/

#ifndef _RANDOM_SYNTHESIZER_HPP
#define _RANDOM_SYNTHESIZER_HPP

#include "Synthesizer.hpp"

class RandomSynthesizer : public Synthesizer
{
public:
    RandomSynthesizer(const Texture & source);
    virtual ~RandomSynthesizer(void);

    virtual string Synthesize(const Position & position, Texture & target) const;

protected:
    const Texture & _source;
};

#endif
