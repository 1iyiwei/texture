/*
  RandomSynthesizer.cpp

  Li-Yi Wei
  August 17, 2014

*/

#include "RandomSynthesizer.hpp"
#include "Random.hpp"

RandomSynthesizer::RandomSynthesizer(const Texture & source) : _source(source)
{
    // nothing else to do
}

RandomSynthesizer::~RandomSynthesizer(void)
{
    // nothing else to do
}

string RandomSynthesizer::Synthesize(const Position & target_position, Texture & target) const
{
    Position source_position(_source.Dimension());

    for(unsigned int dim = 0; dim < source_position.size(); dim++)
    {
        source_position[dim] = Random::UniformInt(0, _source.Size(dim)-1);
    }

    TexelPtr source_texel = 0;
    if(! _source.Get(source_position, source_texel))
    {
        return "RandomSynthesizer::Synthesize(): cannot get source";
    }

    TexelPtr target_texel = 0;
    if(! target.Get(target_position, target_texel))
    {
        return "RandomSynthesizer::Synthesize(): cannot get target";
    }

    if((target_texel == 0) || (target_texel->GetStatus() == Texel::FREE))
    {
        if(! target.Put(target_position, source_texel))
        {
            return "RandomSynthesizer::Synthesize(): cannot put free texel";
        }
    }
    else
    {
        // hard constraint
        const RangePtr source_range = source_texel->GetRange();
        const RangePtr target_range = target_texel->GetRange();

        if(! (source_range && target_range)) return false;

        if(target_range->Distance2(*source_range) <= 0)
        {
            // target_texel->PutRange(source_texel->GetRange()); // tautology
            target_texel->SetPosition(source_texel->GetPosition());
            if(! target.Put(target_position, target_texel))
            {
                return "RandomSynthesizer::Synthesize(): cannot put constrained texel";
            }
        }
    }

    return "";
}
