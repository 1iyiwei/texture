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

RandomDiffusionSequencer::RandomDiffusionSequencer(const vector<Position> & seeds, const Neighborhood & boundary_handler): _seeds(seeds), _neighborhood(boundary_handler)
{
    // nothing else to do
}

RandomDiffusionSequencer::~RandomDiffusionSequencer(void)
{
    // nothing else to do
}

string RandomDiffusionSequencer::Synthesize(const Synthesizer & synthesizer, Texture & target) const
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

    // sort according to distance from the nearest seed
    if(_seeds.size() > 0)
    {
        vector<Sortee> sorted(positions.size());

        for(unsigned int i = 0; i < sorted.size(); i++)
        {
            sorted[i].value = positions[i];
            sorted[i].key = -1;

            for(unsigned int j = 0; j < _seeds.size(); j++)
            {
                const int dist2 = _neighborhood.GetDomain().Geodesic2(target, _seeds[j], positions[i]);
            
                if((sorted[i].key < 0) || (sorted[i].key > dist2))
                {
                    sorted[i].key = dist2;
                }
            }
        }

        sort(sorted.begin(), sorted.end());
   
        for(unsigned int k = 0; k < positions.size(); k++)
        {
            positions[k] = sorted[k].value;
        }
    }

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

bool RandomDiffusionSequencer::Sortee::operator<(const RandomDiffusionSequencer::Sortee & rhs)
{
    return(key < rhs.key);
}
