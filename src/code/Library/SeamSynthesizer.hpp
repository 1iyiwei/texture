/*
  SeamSynthesizer.hpp

  proxy synthesizer that operates only over seams

  Li-Yi Wei
  April 24, 2018

*/

#ifndef _SEAM_SYNTHESIZER_HPP
#define _SEAM_SYNTHESIZER_HPP

#include "Synthesizer.hpp"
#include "Seamster.hpp"

class SeamSynthesizer : public Synthesizer
{
public:
    SeamSynthesizer(const Synthesizer & synthesizer, const Seamster & seamster);
    virtual ~SeamSynthesizer(void);

    virtual string Synthesize(const Position & position, Texture & target) const;

protected:
    const Synthesizer & _synthesizer;
    const Seamster & _seamster;
};

#endif
