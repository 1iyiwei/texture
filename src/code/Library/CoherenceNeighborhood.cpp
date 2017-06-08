/*
  CoherenceNeighborhood.cpp

  Li-Yi Wei
  October 16, 2014

*/

#include <set>
#include "CoherenceNeighborhood.hpp"
#include "Exception.hpp"

CoherenceNeighborhood::CoherenceNeighborhood(const Texture & source, const Neighborhood & input_neighborhood, const Neighborhood & output_neighborhood, const Neighborhood & coherence_neighborhood): Neighborhood(coherence_neighborhood.GetDomain()), _source_texture(source), _input_neighborhood(input_neighborhood), _output_neighborhood(output_neighborhood), _coherence_neighborhood(coherence_neighborhood), _input_pyramid_neighborhood_ptr(dynamic_cast<const PyramidNeighborhood *>(&input_neighborhood)), _output_pyramid_neighborhood_ptr(dynamic_cast<const PyramidNeighborhood *>(&output_neighborhood)), _coherence_pyramid_neighborhood_ptr(dynamic_cast<const PyramidNeighborhood *>(&coherence_neighborhood))
{
    // nothing else to do
}
    
CoherenceNeighborhood::CoherenceNeighborhood(const Texture & source, const PyramidNeighborhood & input_neighborhood, const PyramidNeighborhood & output_neighborhood, const PyramidNeighborhood & coherence_neighborhood): Neighborhood(coherence_neighborhood.GetDomain()), _source_texture(source), _input_neighborhood(input_neighborhood), _output_neighborhood(output_neighborhood), _coherence_neighborhood(coherence_neighborhood), _input_pyramid_neighborhood_ptr(&input_neighborhood), _output_pyramid_neighborhood_ptr(&output_neighborhood), _coherence_pyramid_neighborhood_ptr(&coherence_neighborhood)
{
    // nothing else to do
}

CoherenceNeighborhood::~CoherenceNeighborhood(void)
{
    // nothing else to do
}

vector<Neighborhood::Neighbor> CoherenceNeighborhood::Neighbors(const Texture & source, const Position & position) const
{
    return _coherence_neighborhood.Neighbors(source, position);
}

vector<Position> CoherenceNeighborhood::Candidates(const Texture & texture, const Position & query) const
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
                throw Exception("CoherenceNeighborhood::Candidates(): source.size() != offset.size()");
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
                    throw Exception("CoherenceNeighborhood::Candidates(): need pyramid neighborhood");
                }

                // output is not in the same pyramid level
                // need transport in the input pyramid domain
                const TexturePyramid & input_pyramid = _input_pyramid_neighborhood_ptr->GetPyramid();
                const vector<int> & input_level_size = input_pyramid.Size(level);
                const vector<int> & texture_size = texture.Size();

                const PyramidDomain & input_pyramid_domain = _input_pyramid_neighborhood_ptr->GetPyramidDomain();
                if(! input_pyramid_domain.Trace(input_level_size, output, texture_size, output))
                {
                    throw Exception("CoherenceNeighborhood::Candidates(): cannot transport in the input pyramid domain");
                }

                // transport query back and forth to compute same-level offset
                // in the output pyramid domain
                const TexturePyramid & output_pyramid = _output_pyramid_neighborhood_ptr->GetPyramid();
                const vector<int> & output_level_size = output_pyramid.Size(level);
                const PyramidDomain & output_pyramid_domain = _output_pyramid_neighborhood_ptr->GetPyramidDomain();

                Position query_again = query;
                if(! output_pyramid_domain.Trace(texture_size, query, output_level_size, query_again))
                {
                    throw Exception("CoherenceNeighborhood::Candidates(): cannot transport in the output pyramid domain");
                }
                if(! output_pyramid_domain.Trace(output_level_size, query_again, texture_size, query_again))
                {
                    throw Exception("CoherenceNeighborhood::Candidates(): cannot transport again in the output pyramid domain");
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

set<Position> & CoherenceNeighborhood::PositionSet(const vector<Position> & input, set<Position> & output) const
{
    output.clear();

    for(unsigned int k = 0; k < input.size(); k++)
    {
        output.insert(input[k]);
    }

    return output;
}
