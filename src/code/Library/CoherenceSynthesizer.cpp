/*
  CoherenceSynthesizer.cpp

  Li-Yi Wei
  August 21, 2014

*/

#include "CoherenceSynthesizer.hpp"
#include "Random.hpp"
#include "Math.hpp"
#include "Exception.hpp"

CoherenceSynthesizer::CoherenceSynthesizer(const Texture & source, const Neighborhood & input_neighborhood, const Neighborhood & output_neighborhood, const Neighborhood & coherence_neighborhood, const RangePtr & penalty_range, const RangePtr & zero_range, const int num_extra_random_positions) : _source_texture(source), _input_neighborhood(input_neighborhood), _output_neighborhood(output_neighborhood), _coherence_neighborhood(CoherenceNeighborhood(source, input_neighborhood, output_neighborhood, coherence_neighborhood)), _penalty_range(penalty_range), _zero_range(zero_range), _num_extra_random_positions(num_extra_random_positions)
{
    // nothing else to do
}

CoherenceSynthesizer::CoherenceSynthesizer(const Texture & source, const PyramidNeighborhood & input_neighborhood, const PyramidNeighborhood & output_neighborhood, const PyramidNeighborhood & coherence_neighborhood, const RangePtr & penalty_range, const RangePtr & zero_range, const int num_extra_random_positions): _source_texture(source), _input_neighborhood(input_neighborhood), _output_neighborhood(output_neighborhood), _coherence_neighborhood(CoherenceNeighborhood(source, input_neighborhood, output_neighborhood, coherence_neighborhood)), _penalty_range(penalty_range), _zero_range(zero_range), _num_extra_random_positions(num_extra_random_positions)
{
    // nothing else to do
}

CoherenceSynthesizer::~CoherenceSynthesizer(void)
{
    // nothing else to do
}

string CoherenceSynthesizer::Synthesize(const Position & target_position, Texture & target_texture) const
{
    if(static_cast<int>(target_position.size()) != target_texture.Dimension())
    {
        return "CoherenceSynthesizer::Synthesize(): dimensionality mismatch";
    }

    // construct candidate positions
    vector<Position> candidate_sources = _coherence_neighborhood.Candidates(target_texture, target_position);

    for(int k = 0; k < _num_extra_random_positions; k++)
    {
        Position random_position(target_position);

        for(unsigned int i = 0; i < random_position.size(); i++)
        {
            random_position[i] = Random::UniformInt(0, _source_texture.Size(i)-1);
        }

        candidate_sources.push_back(random_position);
    }

    // target neighbors
    const vector<Neighborhood::Neighbor> target_neighbors_pristine = _output_neighborhood.Neighbors(target_texture, target_position);

    // target itself
    TexelPtr target_texel = 0;
    if(! target_texture.Get(target_position, target_texel))
    {
        return "CoherenceSynthesizer::Synthesize(): cannot get target texel";
    }

    const bool hard_constraint = (target_texel && ((target_texel->GetStatus() == Texel::FIXED)));

    // find best source
    const Position * best_source = 0;
    DistType best_measure = static_cast<DistType>(Math::INF);

    for(unsigned int k = 0; k < candidate_sources.size(); k++)
    {
        const Position & source_position = candidate_sources[k];

        if(! _source_texture.Inside(source_position)) continue;

        if(hard_constraint)
        {
            TexelPtr source_texel = 0;
            if(! _source_texture.Get(source_position, source_texel))
            {   
                return "CoherenceSynthesizer::Synthesize(): cannot get source texel";
            }

            const RangePtr source_range = source_texel->GetRange();
            const RangePtr target_range = target_texel->GetRange();

            if(! (source_range && target_range))
            {
                return "CoherenceSynthesizer::Synthesize(): null source or target range";
            }

            // skip guys who do not satisfy the hard constraint
            if(target_range->Distance2(*source_range) > 0) continue;
        }

        vector<Neighborhood::Neighbor> source_neighbors = _input_neighborhood.Neighbors(_source_texture, source_position);

        vector<Neighborhood::Neighbor> target_neighbors(target_neighbors_pristine);

        if(! Penalize(source_neighbors, target_neighbors))
        {
            return "CoherenceSynthesizer::Synthesize(): cannot penalize";
        }

        const DistType current_measure = Distance2(source_neighbors, target_neighbors);

        if(current_measure < 0)
        {
            return "CoherenceSynthesizer::Synthesize(): negative measure";
        }

        if(current_measure < best_measure)
        {
            best_measure = current_measure;
            best_source = &source_position;
        }
    }

    if(! best_source)
    {
#if 1
        // hard_constraint should not be a failure
        return (hard_constraint ? "" : "CoherenceSynthesizer::Synthesize(): cannot get best source texel for non-hard-constraint");
#else
        return "CoherenceSynthesizer::Synthesize(): cannot find suitable source";
#endif
    }
    else
    {
        TexelPtr source_texel = 0;

        if(! _source_texture.Get(*best_source, source_texel))
        {
            return "CoherenceSynthesizer::Synthesize(): cannot get best source texel";
        }

        if(hard_constraint)
        {
            target_texel->SetPosition(source_texel->GetPosition());
            if(! target_texture.Put(target_position, target_texel))
            {
                return "CoherenceSynthesizer::Synthesize(): cannot put constrained texel";
            }
        }
        else
        {
            if(! target_texture.Put(target_position, source_texel))
            {
                return "CoherenceSynthesizer::Synthesize(): cannot put free texel";
            }
        }

        return "";
    }

    // shouldn't reach here
    return "CoherenceSynthesizer::Synthesize(): shouldn't reach here";
}

bool CoherenceSynthesizer::Penalize(vector<Neighborhood::Neighbor> & source, vector<Neighborhood::Neighbor> & target) const
{
    if(source.size() != target.size())
    {
        return false;
    }

    for(unsigned int k = 0; k < source.size(); k++)
    {
        if(!source[k].texel && target[k].texel)
        {
            // arbitrary position for source
            source[k].texel.reset(new Texel(_penalty_range, target[k].texel->GetPosition()));
            
            target[k].texel.reset(new Texel(_zero_range, target[k].texel->GetPosition()));
        }
    }

    return true;
}
