/*
  Texture.hpp

  Li-Yi Wei
  August 17, 2014

*/

#ifndef _TEXTURE_HPP
#define _TEXTURE_HPP

#include <memory>
using namespace std;

#include "Texel.hpp"
#include "Array.hpp"
#include "FrameBuffer.hpp"

class Texture : public Array<TexelPtr>
{
public:
    Texture(void);
    Texture(const vector<int> & size);
    Texture(const Array<FrameBuffer::P3> & rgb_input);
    Texture(const Array<FrameBuffer::P3> & rgb_input, const Array<Position> & init_coord);
    virtual ~Texture(void);

    bool Convert(Array<FrameBuffer::P3> & rgb_output) const;

protected:
    void Construct(const Array<FrameBuffer::P3> & rgb_input, const Array<Position> & init_coord);
};

typedef shared_ptr<Texture> TexturePtr;

#endif
