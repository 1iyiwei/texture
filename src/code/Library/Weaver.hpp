/*
  Weaver.hpp

  combines sequencer and synthesizer to produce the texture

  Li-Yi Wei
  April 27, 2018 (code restructuring)

*/

#ifndef _WEAVER_HPP
#define _WEAVER_HPP

#include "Sequencer.hpp"
#include "Synthesizer.hpp"

class Weaver
{
public:
    virtual ~Weaver(void);

    // return empty string if successful, error message otherwise
    string Synthesize(Sequencer & sequencer, const Synthesizer & synthesizer, Texture & target) const;
};

#endif
