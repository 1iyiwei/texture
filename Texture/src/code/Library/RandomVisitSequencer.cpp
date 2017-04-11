/*
  RandomVisitSequencer.cpp

  Li-Yi Wei
  August 23, 2014

*/

#include "RandomVisitSequencer.hpp"
#include "Random.hpp"

string RandomVisitSequencer::Synthesize(const Synthesizer & synthesizer, Texture & target) const
{
    // compute total number of target texels
    const vector<int> target_size = target.Size();
    
    int total_count = 1;
    for(unsigned int k = 0; k < target_size.size(); k++)
    {
        total_count *= target_size[k];
    }

    // synthesis
    
    Position index(target.Dimension());

    for(int k = 0; k < total_count; k++)
    {
        for(unsigned int i = 0; i < index.size(); i++)
        {
            index[i] = Random::UniformInt(0, target.Size(i)-1);
        }

        const string message = synthesizer.Synthesize(index, target);

        if(message != "")
        {
            return "";
        }
    }

    // done
    return "";
}
