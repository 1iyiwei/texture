/*
  RandomDiffusionSequencer.cpp

  Li-Yi Wei
  August 27, 2014

*/

#include <algorithm>
using namespace std;

#include "RandomDiffusionSequencer.hpp"
#include "Utility.hpp"
#include "SequentialCounter.hpp"

RandomDiffusionSequencer::RandomDiffusionSequencer(const vector<Position> & seeds, const Neighborhood & boundary_handler): _seeds(seeds), _neighborhood(boundary_handler), _current_position(0)
{
    // nothing else to do
}

RandomDiffusionSequencer::~RandomDiffusionSequencer(void)
{
    // nothing else to do
}

bool RandomDiffusionSequencer::Reset(const Texture & target)
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

    // sort according to distance from the nearest seed
    if(_seeds.size() > 0)
    {
        vector<Sortee> sorted(_positions.size());

        for(unsigned int i = 0; i < sorted.size(); i++)
        {
            sorted[i].value = _positions[i];
            sorted[i].key = -1;

            for(unsigned int j = 0; j < _seeds.size(); j++)
            {
                const int dist2 = _neighborhood.GetDomain().Geodesic2(target, _seeds[j], _positions[i]);
            
                if((sorted[i].key < 0) || (sorted[i].key > dist2))
                {
                    sorted[i].key = dist2;
                }
            }
        }

        sort(sorted.begin(), sorted.end());
   
        for(unsigned int k = 0; k < _positions.size(); k++)
        {
            _positions[k] = sorted[k].value;
        }
    }

    _current_position = 0;

    return true;
}

bool RandomDiffusionSequencer::Next(Position & answer)
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

bool RandomDiffusionSequencer::Sortee::operator<(const RandomDiffusionSequencer::Sortee & rhs)
{
    return(key < rhs.key);
}
