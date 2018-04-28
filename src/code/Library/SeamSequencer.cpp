/*
  SeamSequencer.cpp

  Li-Yi Wei
  April 23, 2018

*/

#include "SeamSequencer.hpp"

SeamSequencer::SeamSequencer(const Seamster & seamster, shared_ptr<Sequencer> & source): _seamster(seamster), _source(source), _current_position(0)
{
    // nothing else to do
}

SeamSequencer::~SeamSequencer(void)
{
    // nothing else to do
}

bool SeamSequencer::Reset(const Texture & target)
{
    if(!_source)
    {
        // null source
        return false;
    }

    if(!_source->Reset(target))
    {
        return false;
    }

    Position position;
    _positions.clear();

    while(_source->Next(position))
    {
        if(_seamster.AtSeam(position, target))
        {
            _positions.push_back(position);
        }
    }
    
    _current_position = 0;

    // done
    return true;
}

bool SeamSequencer::Next(Position & answer)
{
    if(_current_position < _positions.size())
    {
        answer = _positions[_current_position];
        _current_position++;
        return true;
    }
    else
    {
        return false;
    }
}
