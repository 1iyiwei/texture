/*
  RandomVisitSequencer.cpp

  Li-Yi Wei
  August 23, 2014

*/

#include "RandomVisitSequencer.hpp"
#include "Random.hpp"

RandomVisitSequencer::RandomVisitSequencer(void): _total_count(0)
{
    // nothing else to do
}

bool RandomVisitSequencer::Reset(const Texture & target)
{
    // compute total number of target texels
    _target_size = target.Size();
    
    _total_count = 1;
    for(unsigned int k = 0; k < _target_size.size(); k++)
    {
        _total_count *= _target_size[k];
    }

    return true;
}

bool RandomVisitSequencer::Next(Position & answer)
{
    if(_total_count <= 0)
    {
        return false;
    }
    else
    {
        _total_count--;
        
        if(answer.size() != _target_size.size())
        {
            answer = Position(_target_size.size());
        }

        for(unsigned int i = 0; i < answer.size(); i++)
        {
            answer[i] = Random::UniformInt(0, _target_size[i]-1);
        }

        return true;
    }
}
