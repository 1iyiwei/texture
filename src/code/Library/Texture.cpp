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
    Array<Position> init_coord(rgb_input.Size());

    const int dimension = rgb_input.Dimension();

    SequentialCounter counter(dimension, vector<int>(dimension, 0), Utility::Minus1(init_coord.Size()));

    Position position;

    counter.Reset();
    do
    {
        counter.Get(position);
        
        if(! init_coord.Put(position, position))
        {
            throw Exception("Texture::Texture(): cannot put position");
        }
    }
    while(counter.Next());

    Construct(rgb_input, init_coord);
}

void Texture::Construct(const Array<FrameBuffer::P3> & rgb_input, const Array<Position> & init_coord)
{
    if(rgb_input.Dimension() != init_coord.Dimension())
    {
        throw Exception("Texture::Construct(): dimensionality mismatch");
    }

    const int dimension = rgb_input.Dimension();

    SequentialCounter counter(dimension, vector<int>(dimension, 0), Utility::Minus1(init_coord.Size()));

    Position position, coord;
    FrameBuffer::P3 pixel;
    vector<int> vecxel(3);
    
    counter.Reset();
    do
    {
        counter.Get(position);

        if(! init_coord.Get(position, coord))
        {
            throw Exception("Texture::Construct(): cannot get coord");
        }

        if(! rgb_input.Get(coord, pixel))
        {
            throw Exception("Texture::Construct(): cannot get pixel");
        }

        vecxel[0] = pixel.r; vecxel[1] = pixel.g; vecxel[2] = pixel.b;
        RangePtr range(new VectorRange<int>(vecxel));
        
        TexelPtr texel(new Texel(range, coord));

        if(! Put(position, texel))
        {
            throw Exception("Texture::Construct(): cannot put texel");
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
