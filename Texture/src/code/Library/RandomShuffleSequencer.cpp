/*
  RandomShuffleSequencer.cpp

  Li-Yi Wei
  August 23, 2014

*/

#include <algorithm>
using namespace std;

#include "RandomShuffleSequencer.hpp"
#include "SequentialCounter.hpp"
#include "Utility.hpp"

string RandomShuffleSequencer::Synthesize(const Synthesizer & synthesizer, Texture & target) const
{
    // build list of positions
    const int dimension = target.Dimension();

    const vector<int> min_index = vector<int>(dimension, 0);
    const vector<int> max_index = Utility::Minus1(target.Size());

    SequentialCounter counter(dimension, min_index, max_index);

    Position index;
    vector<Position> positions;
    counter.Reset();
    do
    {
        counter.Get(index);
        positions.push_back(index);
    }
    while(counter.Next());

    // random shuffle
    random_shuffle(positions.begin(), positions.end());

    // synthesis
    for(unsigned int k = 0; k < positions.size(); k++)
    {
        const vector<int> & index = positions[k];

        const string message = synthesizer.Synthesize(index, target);

        if(message != "")
        {
            return message;
        }
    }
    
    // done
    return "";
}
