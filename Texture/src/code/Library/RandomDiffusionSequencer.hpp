/*
  RandomDiffusionOrder.hpp

  process pixels in a diffusion order starting from constraints
  but randomized otherwise

  Li-Yi Wei
  August 27, 2014

*/

#ifndef _RANDOM_DIFFUSION_SEQUENCER_HPP
#define _RANDOM_DIFFUSION_SEQUENCER_HPP

#include <vector>
using namespace std;

#include "Sequencer.hpp"
#include "Neighborhood.hpp"

class RandomDiffusionSequencer : public Sequencer
{
public:
    RandomDiffusionSequencer(const vector<Position> & seeds, const Neighborhood & boundary_handler);
    virtual ~RandomDiffusionSequencer(void);

    virtual string Synthesize(const Synthesizer & synthesizer, Texture & target) const;

protected:
    const vector<Position> _seeds;
    const Neighborhood & _neighborhood;

    struct Sortee
    {
        int key;
        Position value;
        bool operator<(const Sortee & rhs);
    };
};
#endif
