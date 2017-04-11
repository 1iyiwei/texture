/*
  ScanlineSequencer.cpp

  Li-Yi Wei
  August 17, 2014

*/

#include <algorithm>
using namespace std;

#include "ScanlineSequencer.hpp"
#include "SequentialCounter.hpp"
#include "Utility.hpp"

string ScanlineSequencer::Synthesize(const Synthesizer & synthesizer, Texture & target) const
{
    const int dimension = target.Dimension();

    vector<int> min_index = vector<int>(dimension, 0);
    vector<int> max_index = Utility::Minus1(target.Size());
    Reverse(max_index);

    SequentialCounter counter(dimension, min_index, max_index);

    Position index;

    counter.Reset();

    do
    {
        counter.Get(index);

        Reverse(index);

        const string message = synthesizer.Synthesize(index, target);

        if(message != "")
        {
            return message;
        }
    }        
    while(counter.Next());

    return "";
}

void ScanlineSequencer::Reverse(Position & position)
{
    reverse(position.begin(), position.end());
}
