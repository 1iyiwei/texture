/*
  Texture.cpp

  Li-Yi Wei
  August 17, 2014

*/

#include "Texture.hpp"
#include "VectorRange.hpp"
#include "SequentialCounter.hpp"
#include "Utility.hpp"
#include "Exception.hpp"

Texture::Texture(void)
{
    // nothing else to do
}

Texture::Texture(const vector<int> & size) : Array<TexelPtr>(size)
{
    // nothing else to do
}

Texture::Texture(const Array<FrameBuffer::P3> & rgb_input) : Array<TexelPtr>(rgb_input.Size())
{
    const int dimension = rgb_input.Dimension();

    SequentialCounter counter(dimension, vector<int>(dimension, 0), Utility::Minus1(rgb_input.Size()));

    Position position;
    FrameBuffer::P3 pixel;
    vector<int> vecxel(3);
    
    counter.Reset();
    do
    {
        counter.Get(position);

        if(! rgb_input.Get(position, pixel))
        {
            throw Exception("Texture::Texture(const Array<FrameBuffer::P3> &): cannot get pixel");
        }

        vecxel[0] = pixel.r; vecxel[1] = pixel.g; vecxel[2] = pixel.b;
        RangePtr range(new VectorRange<int>(vecxel));
        
        TexelPtr texel(new Texel(range, position));

        if(! Put(position, texel))
        {
            throw Exception("Texture::Texture(const Array<FrameBuffer::P3> &): cannot put texel");
        }
    }
    while(counter.Next());
}

Texture::~Texture(void)
{
    // nothing else to do
}

bool Texture::Convert(Array<FrameBuffer::P3> & rgb_output) const
{
    vector<TexelPtr> texels;
    if(! Get(texels)) return false;

    rgb_output = Array<FrameBuffer::P3>(Size());

    vector<FrameBuffer::P3> pixels;
    if(! rgb_output.Get(pixels)) return false;

    if(texels.size() != pixels.size()) return false;

    for(unsigned int k = 0; k < pixels.size(); k++)
    {
        if(! texels[k]) return false;

        const RangePtr range = texels[k]->GetRange();

        const VectorRange<int> * vector_range = dynamic_cast<VectorRange<int> *>(range.get());

        if(!vector_range) return false;
        if(vector_range->Length() != 3) return false;

        if(!vector_range->Get(0, pixels[k].r) || !vector_range->Get(1, pixels[k].g) || !vector_range->Get(2, pixels[k].b)) return false;
    }

    return rgb_output.Put(pixels);
}
