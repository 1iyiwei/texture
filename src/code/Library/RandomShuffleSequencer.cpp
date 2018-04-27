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

RandomShuffleSequencer::RandomShuffleSequencer(void): _current_position(0)
{
    // nothing else to do
}

bool RandomShuffleSequencer::Reset(const Texture & target)
{
    // build list of positions
    const int dimension = target.Dimension();

    const vector<int> min_index = vector<int>(dimension, 0);
    const vector<int> max_index = Utility::Minus1(target.Size());

    SequentialCounter counter(dimension, min_index, max_index);

    Position index;
    _positions.clear();
    counter.Reset();
    do
    {
        counter.Get(index);
        _positions.push_back(index);
    }
    while(counter.Next());

    // random shuffle
    random_shuffle(_positions.begin(), _positions.end());

    _current_position = 0;

    return true;
}

bool RandomShuffleSequencer::Next(Position & answer)
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
