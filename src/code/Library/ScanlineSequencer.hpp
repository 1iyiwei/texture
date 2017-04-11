/*
  ScanlineSequencer.hpp

  Li-Yi Wei
  August 17, 2014

*/

#ifndef _SCANLINE_SEQUENCER_HPP
#define _SCANLINE_SEQUENCER_HPP

#include "Sequencer.hpp"

class ScanlineSequencer : public Sequencer
{
public:
    virtual string Synthesize(const Synthesizer & synthesizer, Texture & target) const;

protected:
    static void Reverse(Position & position);
};
#endif
