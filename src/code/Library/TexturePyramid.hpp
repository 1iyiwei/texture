/*
  TexturePyramid.hpp

  Li-Yi Wei
  October 16, 2014

*/

#ifndef _TEXTURE_PYRAMID_HPP
#define _TEXTURE_PYRAMID_HPP

#include <memory>
using namespace std;

#include "Texture.hpp"
#include "Pyramid.hpp"

class TexturePyramid : public Pyramid<TexelPtr>
{
public:
    
    TexturePyramid(void);
    TexturePyramid(const vector<TexturePtr> & data);
    TexturePyramid(const vector< vector<int> > & level_sizes);
    TexturePyramid(const vector<int> & top_level_size, const int num_levels);
    TexturePyramid(const TexturePyramid & input, const int start_level, const int end_level);
    virtual ~TexturePyramid(void);

    TexturePtr Get(const int level) const;
    bool Put(const int level, const TexturePtr & input);
    
    vector<TexturePtr> Get(const int start_level, const int end_level) const;

protected:
    static vector<ArrayPtr> Convert(const vector<TexturePtr> & input);
    static vector<TexturePtr> Construct(const vector< vector<int> > & level_sizes);
};

typedef shared_ptr<TexturePyramid> TexturePyramidPtr;
#endif
