/*
  SeamSequencer.hpp

  only visit texels that are on seams, with discontinuous source coordinates

  Li-Yi Wei
  April 23, 2018

*/

#ifndef _SEAM_SEQUENCER_HPP
#define _SEAM_SEQUENCER_HPP

#include "Sequencer.hpp"

class SeamSequencer : public Sequencer
{
public:
    virtual ~SeamSequencer(void);

    virtual bool Reset(const Texture & target);
    virtual bool Next(Position & answer);
};
#endif
