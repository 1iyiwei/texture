/*
  SeamSequencer.hpp

  only visit texels that are on seams, with discontinuous source coordinates

  Li-Yi Wei
  April 23, 2018

*/

#ifndef _SEAM_SEQUENCER_HPP
#define _SEAM_SEQUENCER_HPP

#include "Seamster.hpp"
#include "Sequencer.hpp"

class SeamSequencer : public Sequencer
{
public:
    // take orders from the source sequencer and keep only seams
    SeamSequencer(const Seamster & seamster, shared_ptr<Sequencer> & source);
    virtual ~SeamSequencer(void);

    virtual bool Reset(const Texture & target);
    virtual bool Next(Position & answer);

private:
    const Seamster & _seamster;
    shared_ptr<Sequencer> _source;
    vector<Position> _positions;
    unsigned int _current_position; 
};
#endif
