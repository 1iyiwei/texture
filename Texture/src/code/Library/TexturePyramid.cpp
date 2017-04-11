/*
  TexturePyramid.cpp

  Li-Yi Wei
  October 16, 2014

*/

#include "TexturePyramid.hpp"
 
TexturePyramid::TexturePyramid(void)
{
    // nothing else to do
}

TexturePyramid::TexturePyramid(const vector<TexturePtr> & data): Pyramid(Convert(data))
{
    // nothing else to do
}

TexturePyramid::TexturePyramid(const vector< vector<int> > & level_sizes): Pyramid(Convert(Construct(level_sizes)))
{
    // nothing else to do
}

TexturePyramid::TexturePyramid(const vector<int> & top_level_size, const int num_levels) : Pyramid(Convert(Construct(DefaultLevelSizes(top_level_size, num_levels))))
{
    // nothing else to do
}

TexturePyramid::TexturePyramid(const TexturePyramid & input, const int start_level, const int end_level) : Pyramid(Convert(input.Get(start_level, end_level)))
{
    // nothing else to do
}

TexturePyramid::~TexturePyramid(void)
{
    // nothing else to do
}

vector<Pyramid<TexelPtr>::ArrayPtr> TexturePyramid::Convert(const vector<TexturePtr> & input)
{
    vector<ArrayPtr> output(input.size());

    for(unsigned int i = 0; i < output.size(); i++)
    {
        output[i] = input[i];
    }

    return output;
}

vector<TexturePtr> TexturePyramid::Construct(const vector< vector<int> > & level_sizes)
{
    vector<TexturePtr> output;

    for(unsigned int i = 0; i < level_sizes.size(); i++)
    {
        const vector<int> & level_size = level_sizes[i];

        // check
        for(unsigned int k = 0; k < level_size.size(); k++)
        {
            if(level_size[k] <= 0)
            {
                throw Exception("TexturePyramid::Convert(): illegal level size");
            }
        }

        // build
        TexturePtr level(new Texture(level_size));
        output.push_back(level);
    }

    return output;
}

TexturePtr TexturePyramid::Get(const int level) const
{
    if(LegalLevel(level))
    {
        return dynamic_pointer_cast<Texture>(_data[level]);
    }
    else
    {
        return 0;
    }
}

bool TexturePyramid::Put(const int level, const TexturePtr & input)
{
    return Pyramid::Put(level, dynamic_pointer_cast<Array<TexelPtr> >(input));
}
   
vector<TexturePtr> TexturePyramid::Get(const int start_level, const int end_level) const
{
    vector<TexturePtr> answer;

    for(int level = start_level; level <= end_level; level++)
    {
        answer.push_back(Get(level));
    }

    return answer;
}
