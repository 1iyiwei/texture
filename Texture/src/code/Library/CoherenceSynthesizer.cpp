/*
  CoherenceSynthesizer.cpp

  Li-Yi Wei
  August 21, 2014

*/

#include "CoherenceSynthesizer.hpp"
#include "Random.hpp"
#include "Math.hpp"
#include "Exception.hpp"

CoherenceSynthesizer::CoherenceSynthesizer(const Texture & source, const Neighborhood & input_neighborhood, const Neighborhood & output_neighborhood, const Neighborhood & coherence_neighborhood, const RangePtr & penalty_range, const RangePtr & zero_range, const int num_extra_random_positions) : _source_texture(source), _input_neighborhood(input_neighborhood), _output_neighborhood(output_neighborhood), _coherence_neighborhood(coherence_neighborhood), _input_pyramid_neighborhood_ptr(dynamic_cast<const PyramidNeighborhood *>(&input_neighborhood)), _output_pyramid_neighborhood_ptr(dynamic_cast<const PyramidNeighborhood *>(&output_neighborhood)), _coherence_pyramid_neighborhood_ptr(dynamic_cast<const PyramidNeighborhood *>(&coherence_neighborhood)), _penalty_range(penalty_range), _zero_range(zero_range), _num_extra_random_positions(num_extra_random_positions)
{
    // nothing else to do
}

CoherenceSynthesizer::CoherenceSynthesizer(const Texture & source, const PyramidNeighborhood & input_neighborhood, const PyramidNeighborhood & output_neighborhood, const PyramidNeighborhood & coherence_neighborhood, const RangePtr & penalty_range, const RangePtr & zero_range, const int num_extra_random_positions): _source_texture(source), _input_neighborhood(input_neighborhood), _output_neighborhood(output_neighborhood), _coherence_neighborhood(coherence_neighborhood), _input_pyramid_neighborhood_ptr(&input_neighborhood), _output_pyramid_neighborhood_ptr(&output_neighborhood), _coherence_pyramid_neighborhood_ptr(&coherence_neighborhood), _penalty_range(penalty_range), _zero_range(zero_range), _num_extra_random_positions(num_extra_random_positions)
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
    vector<Position> candidate_sources = CoherenceCandidates(target_texture, target_position);

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

vector<Position> CoherenceSynthesizer::CoherenceCandidates(const Texture & texture, const Position & query) const
{
    vector<Position> candidates;

    const vector<Neighborhood::Neighbor> neighbors = _coherence_neighborhood.Neighbors(texture, query);

    for(unsigned int k = 0; k < neighbors.size(); k++)
    {
        const ConstTexelPtr & texel = neighbors[k].texel;

        if(texel != 0)
        {
            const Position & source = texel->GetPosition();

            const Position & offset = neighbors[k].offset;

            if(source.size() != offset.size())
            {
#if 1
                throw Exception("CoherenceSynthesizer::CoherenceCandidates(): source.size() != offset.size()");
#else
                // could be constrained texels without actual source position
                continue;
#endif
            }

            Position output = source;

            for(unsigned int i = 0; i < output.size(); i++)
            {
                output[i] = source[i] - offset[i];
            }

            const int level = neighbors[k].level;

            if(level >= 0)
            {
                if(!(_input_pyramid_neighborhood_ptr && _output_pyramid_neighborhood_ptr))
                {
                    throw Exception("CoherenceSynthesizer::CoherenceCandidates(): need pyramid neighborhood");
                }

                // output is not in the same pyramid level
                // need transport in the input pyramid domain
                const TexturePyramid & input_pyramid = _input_pyramid_neighborhood_ptr->GetPyramid();
                const vector<int> & input_level_size = input_pyramid.Size(level);
                const vector<int> & texture_size = texture.Size();

                const PyramidDomain & input_pyramid_domain = _input_pyramid_neighborhood_ptr->GetPyramidDomain();
                if(! input_pyramid_domain.Trace(input_level_size, output, texture_size, output))
                {
                    throw Exception("CoherenceSynthesizer::CoherenceCandidates(): cannot transport in the input pyramid domain");
                }

                // transport query back and forth to compute same-level offset
                // in the output pyramid domain
                const TexturePyramid & output_pyramid = _output_pyramid_neighborhood_ptr->GetPyramid();
                const vector<int> & output_level_size = output_pyramid.Size(level);
                const PyramidDomain & output_pyramid_domain = _output_pyramid_neighborhood_ptr->GetPyramidDomain();

                Position query_again = query;
                if(! output_pyramid_domain.Trace(texture_size, query, output_level_size, query_again))
                {
                    throw Exception("CoherenceSynthesizer::CoherenceCandidates(): cannot transport in the output pyramid domain");
                }
                if(! output_pyramid_domain.Trace(output_level_size, query_again, texture_size, query_again))
                {
                    throw Exception("CoherenceSynthesizer::CoherenceCandidates(): cannot transport again in the output pyramid domain");
                }

                // add together
                for(unsigned int i = 0; i < output.size(); i++)
                {
                    output[i] += query[i] - query_again[i];
                }
            }

            _input_neighborhood.GetDomain().Correct(_source_texture, output);

            candidates.push_back(output);
        }
    }

    {
        set<Position> uniques;
        uniques = PositionSet(candidates, uniques);
        
        candidates.clear();
        for(set<Position>::iterator it = uniques.begin(); it != uniques.end(); ++it)
        {
            candidates.push_back(*it);
        }
    }

    return candidates;
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
