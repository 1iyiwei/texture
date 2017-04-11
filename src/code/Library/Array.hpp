/*
  Array.hpp

  template for arbitrary dimensional arrays

  Li-Yi Wei
  07/08/2007

*/

#ifndef _ARRAY_HPP
#define _ARRAY_HPP

#include <vector>
#include <memory>
using namespace std;

template<class T>
class Array
{
public:
    typedef shared_ptr<Array> Ptr;

public:
    Array(void);
    Array(const vector<int> & spec);
    virtual ~Array(void);

    int Dimension(void) const;
    int Size(const int which_dim) const;
    void Size(vector<int> & answer) const;
    const vector<int> & Size(void) const;

    // both return 1 if operation successful, 0 else
    bool Get(const vector<int> & index, T & output) const;
    bool Put(const vector<int> & index, const T & input);

    // batch mode, handle all
    bool Get(vector<T> & output) const;
    bool Put(const vector<T> & input);
    
    bool Inside(const vector<int> & index) const;

    void Clear(const T & value);
    
protected:
    int ConvertIndex(const vector<int> & index) const;
    
protected:
    vector<int> _size;
    vector<T> _data;
};

template<class T>
Array<T>::Array(void)
{
    // nothing to do
}

template<class T>
Array<T>::Array(const vector<int> & spec) : _size(spec)
{
    int total_count = 1;
    
    for(unsigned int i = 0; i < _size.size(); i++)
    {
        if(_size[i] <= 0)
        {
            _size[i] = 1;
        }

        total_count *= _size[i];
    }

    _data = vector<T>(total_count);
}

template<class T>
Array<T>::~Array(void)
{
    // nothing to do
}

template<class T>
int Array<T>::Dimension(void) const
{
    return _size.size();
}

template<class T>
int Array<T>::Size(const int which_dim) const
{
    if((which_dim >= 0) && (static_cast<unsigned int>(which_dim) < _size.size()))
    {
        return _size[which_dim];
    }
    else
    {
        return -1;
    }
}

template<class T>
void Array<T>::Size(vector<int> & answer) const
{
    answer = _size;
}

template<class T>
const vector<int> & Array<T>::Size(void) const
{
    return _size;
}

template<class T>
bool Array<T>::Get(const vector<int> & index_input, T & output) const
{
    const int index = ConvertIndex(index_input);

    if((index >= 0) && (static_cast<unsigned int>(index) < _data.size()))
    {
        output = _data[index];
        return true;
    }
    else
    {
        return false;
    }
}

template<class T>
bool Array<T>::Put(const vector<int> & index_input, const T & input)
{
    const int index = ConvertIndex(index_input);
    
    if((index >= 0) && (static_cast<unsigned int>(index) < _data.size()))
    {
        _data[index] = input;
        return true;
    }
    else
    {
        return false;
    }
}

template<class T>
bool Array<T>::Get(vector<T> & output) const
{
    output = _data;
    return true;
}

template<class T>
bool Array<T>::Put(const vector<T> & input)
{
    if(input.size() == _data.size())
    {
        _data = input;
        return true;
    }
    else
    {
        return false;
    }
}
   
template<class T>
bool Array<T>::Inside(const vector<int> & index) const
{
    if(index.size() != _size.size())
    {
        return false;
    }
    else
    {
        for(unsigned int i = 0; i < index.size(); i++)
        {
            if((index[i] < 0) || (index[i] >= _size[i]))
            {
                return false;
            }
        }

        return true;
    }
}

template<class T>
void Array<T>::Clear(const T & value)
{
    for(unsigned int i = 0; i < _data.size(); i++)
    {
        _data[i] = value;
    }
}

template<class T>
int Array<T>::ConvertIndex(const vector<int> & index) const
{
    if(index.size() != _size.size())
    {
        return -1;
    }
    else
    {
        {
            for(unsigned int i = 0; i < index.size(); i++)
            {
                if((index[i] < 0) || (index[i] >= _size[i]))
                {
                    return -1;
                }
            }
        }

        int answer = 0;

        for(unsigned int i = 0; i < index.size(); i++)
        {
            answer *= _size[i];
            answer += index[i];
        }

        return answer;
    }
}

#if 0
template<class T>
using ArrayPtr<T> = shared_ptr< Array<T> >;
#endif

#endif
