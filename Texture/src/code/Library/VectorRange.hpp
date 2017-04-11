/*
  VectorRange.hpp

  the vector subclass of Range, representing common color and such

  Li-Yi Wei
  August 17, 2014

*/

#ifndef _VECTOR_RANGE_HPP
#define _VECTOR_RANGE_HPP

#include "Range.hpp"
#include "Math.hpp"
#include "Exception.hpp"

#include <vector>
using namespace std;

template<class T>
class VectorRange : public Range
{
public:
    VectorRange(void);
    VectorRange(const int size);
    VectorRange(const vector<T> & data);
    ~VectorRange(void);

    int Length(void) const;

    bool Get(const int index, T & answer) const;
    bool Put(const int index, const T & item);

    DistType Distance2(const Range & rhs) const;
    DistType Distance2(const VectorRange & rhs) const;

protected:
    vector<T> _data;
};


template<class T>
VectorRange<T>::VectorRange(void)
{
    // nothing else to do
}

template<class T>
VectorRange<T>::VectorRange(const int size) : _data(size)
{
    // nothing else to do
}

template<class T>
VectorRange<T>::VectorRange(const vector<T> & data) : _data(data)
{
    // nothing else to do
}

template<class T>
VectorRange<T>::~VectorRange(void)
{
    // nothing else to do
}

template<class T>
int VectorRange<T>::Length(void) const
{
    return _data.size();
}

template<class T>
bool VectorRange<T>::Get(const int index, T & answer) const
{
    if((index >= 0) && (static_cast<unsigned int>(index) < _data.size()))
    {
        answer = _data[index];
        return true;
    }
    else
    {
        return false;
    }
}

template<class T>
bool VectorRange<T>::Put(const int index, const T & item)
{
    if((index >= 0) && (index < _data.size()))
    {
        _data[index] = item;
        return true;
    }
    else
    {
        return false;
    }
}

template<class T>
Range::DistType VectorRange<T>::Distance2(const Range & rhs) const
{
    const VectorRange<T> * ptr = dynamic_cast<const VectorRange<T> *>(&rhs);

    if(! ptr)
    {
        throw Exception("VectorRange<T>::Distance2(): failure!");
    }

    return Distance2(*ptr);
}

template<class T>
Range::DistType VectorRange<T>::Distance2(const VectorRange & rhs) const
{
    if(_data.size() != rhs._data.size())
    {
        throw Exception("VectorRange<T>::Distance2(): incompatible length!");
        return static_cast<DistType>(Math::INF);
    }

    DistType answer = 0;

    for(unsigned int i = 0; i < _data.size(); i++)
    {
        const DistType diff = static_cast<DistType>(_data[i] - rhs._data[i]);

        answer += diff*diff;
    }

    return answer;
}

#endif
