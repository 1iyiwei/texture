/*
  ScanlineSequencer.hpp

  Li-Yi Wei
  August 17, 2014

*/

#ifndef _SCANLINE_SEQUENCER_HPP
#define _SCANLINE_SEQUENCER_HPP

#include "Sequencer.hpp"
#include "SequentialCounter.hpp"

class ScanlineSequencer : public Sequencer
{
public:
    ScanlineSequencer(void);
    virtual bool Reset(const Texture & target);
    virtual bool Next(Position & answer);

protected:
    static void Reverse(Position & position);

private:
    SequentialCounter _counter;
    bool _over;
};
#endif
