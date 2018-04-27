/*
  Weaver.cpp

  Li-Yi Wei
  April 27, 2018

*/

#include "Weaver.hpp"

Weaver::~Weaver(void)
{
    // nothing else to do
}

string Weaver::Synthesize(Sequencer & sequencer, const Synthesizer & synthesizer, Texture & target) const
{
    if(! sequencer.Reset(target))
    {
        return "cannot reset sequencer";
    }

    Position position;
    string message;
    while(sequencer.Next(position))
    {
        message = synthesizer.Synthesize(position, target);

        if(message != "")
        {
            return message;
        }
    }

    // done
    return "";
}
