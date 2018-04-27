/*
  ScanlineSequencer.cpp

  Li-Yi Wei
  August 17, 2014

*/

#include <algorithm>
using namespace std;

#include "ScanlineSequencer.hpp"
#include "Utility.hpp"

ScanlineSequencer::ScanlineSequencer(void): _over(true)
{
    // nothing else to do
}

bool ScanlineSequencer::Reset(const Texture & target)
{
    const int dimension = target.Dimension();

    vector<int> min_index = vector<int>(dimension, 0);
    vector<int> max_index = Utility::Minus1(target.Size());
    Reverse(max_index);

    _over = false;

    return _counter.Reset(dimension, min_index, max_index);
}

bool ScanlineSequencer::Next(Position & answer)
{
    if(_over)
    {
        return false;
    }
    else
    {
        _counter.Get(answer);
        Reverse(answer);
        _over = !_counter.Next();

        return true;
    }
}

void ScanlineSequencer::Reverse(Position & position)
{
    reverse(position.begin(), position.end());
}
