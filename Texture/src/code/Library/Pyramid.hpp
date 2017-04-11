/*
  Pyramid.hpp

  Li-Yi Wei
  October 16, 2014

*/

#ifndef _PYRAMID_HPP
#define _PYRAMID_HPP

#include "Array.hpp"
#include "PyramidDomain.hpp"
#include "Exception.hpp"

// #define _ARRAY_PTR_GET_INT

template<class T>
class Pyramid
{
public:
    //typedef shared_ptr<Array<T> > ArrayPtr;
    typedef typename Array<T>::Ptr ArrayPtr;

public:
    
    Pyramid(void);
    // allows arbitrary size at each level
    Pyramid(const vector<ArrayPtr> & data); 
    Pyramid(const vector< vector<int> > & level_sizes);
    // default size at each level
    Pyramid(const vector<int> & top_level_size, const int num_levels);
    virtual ~Pyramid(void);

    int Size(void) const; // number of levels
    int Dimension(const int level) const;
    int Size(const int level, const int which_dim) const;
    bool Size(const int level, vector<int> & answer) const;
    const vector<int> & Size(const int level) const;

#ifdef _ARRAY_PTR_GET_INT
    virtual ArrayPtr Get(const int level) const;
#endif
    virtual bool Get(const int level, ArrayPtr & output) const;
    virtual bool Put(const int level, const ArrayPtr & input);
    // separate level from per array index
    virtual bool Get(const int level, const vector<int> & index, T & output) const;
    virtual bool Put(const int level, const vector<int> & index, const T & input);
    // wrap up level into the 0th index element
    bool Get(const vector<int> & index, T & output) const;
    bool Put(const vector<int> & index, const T & input);

protected: // better use PyramidDomain for Trace()

    // trace corresponding indices between levels like ancestry/offspring
    virtual bool Trace(const PyramidDomain & pyramid_domain, const int source_level, const vector<int> & source_index, const int destination_level, vector<int> & destination_index) const;
    // level in 0th index element
    bool Trace(const PyramidDomain & pyramid_domain, const vector<int> & source, vector<int> & destination) const;

protected:
    static vector<ArrayPtr> Construct(const vector< vector<int> > & level_sizes);
    static vector< vector<int> > DefaultLevelSizes(const vector<int> & top_level_size, const int num_levels);

    void Check(void);

    bool LegalLevel(const int level) const;

protected:
    vector<ArrayPtr> _data;
    vector<int> _null_size;
};

template<class T>
Pyramid<T>::Pyramid(void)
{
    // nothing else to do
}

template<class T>
Pyramid<T>::Pyramid(const vector<ArrayPtr> & data): _data(data)
{
    Check();
}
   
template<class T>
Pyramid<T>::Pyramid(const vector< vector<int> > & level_sizes): _data(Construct(level_sizes))
{
    Check();
}

template<class T>
Pyramid<T>::Pyramid(const vector<int> & top_level_size, const int num_levels): _data(Construct(DefaultLevelSizes(top_level_size, num_levels)))
{
    Check();
}

template<class T>
void Pyramid<T>::Check(void)
{
    for(unsigned int i = 0; i < _data.size(); i++)
    {
        if(_data[i] == 0) throw Exception("Pyramid<T>::Pyramid(): null level");
    }
}

template<class T>
Pyramid<T>::~Pyramid(void)
{
    // nothing else to do
}

template<class T>
int Pyramid<T>::Size(void) const
{
    return _data.size();
}

template<class T>
int Pyramid<T>::Dimension(const int level) const
{
    if(LegalLevel(level))
    {
        return _data[level]->Dimension();
    }
    else
    {
        return 0;
    }
}

template<class T>
int Pyramid<T>::Size(const int level, const int which_dim) const
{
    if(LegalLevel(level))
    {
        return _data[level]->Size(which_dim);
    }
    else
    {
        return 0;
    }
}

template<class T>
bool Pyramid<T>::Size(const int level, vector<int> & answer) const
{
    if(LegalLevel(level))
    {
        _data[level]->Size(answer);
        return true;
    }
    else
    {
        return false;
    }
}

template<class T>
const vector<int> & Pyramid<T>::Size(const int level) const
{
    if(LegalLevel(level))
    {
        return _data[level]->Size();
    }
    else
    {
        return _null_size;
    }
}

#ifdef _ARRAY_PTR_GET_INT
template<class T>
typename Pyramid<T>::ArrayPtr Pyramid<T>::Get(const int level) const
{
    if(LegalLevel(level))
    {
        return _data[level];
    }
    else
    {
        return 0;
    }
}
#endif

template<class T>
bool Pyramid<T>::Get(const int level, ArrayPtr & output) const
{
    if(LegalLevel(level))
    {
        output = _data[level];
        return true;
    }
    else
    {
        return false;
    }
}

template<class T>
bool Pyramid<T>::Put(const int level, const ArrayPtr & input)
{
    if(LegalLevel(level))
    {
        _data[level] = input;
        return true;
    }
    else
    {
        return false;
    }
}

template<class T>
bool Pyramid<T>::Get(const int level, const vector<int> & index, T & output) const
{
    if(LegalLevel(level))
    {
        return _data[level]->Get(index,  output);
    }
    else
    {
        return false;
    }
}

template<class T>
bool Pyramid<T>::Put(const int level, const vector<int> & index, const T & input)
{
    if(LegalLevel(level))
    {
        return _data[level]->Put(index, input);
    }
    else
    {
        return false;
    }
}

template<class T>
bool Pyramid<T>::Get(const vector<int> & index, T & output) const
{
    if(index.size() < 1)
    {
        return false;
    }
    else
    {
        return Get(index[0], vector<int>(index.begin()+1, index.end()), output);
    }
}

template<class T>
bool Pyramid<T>::Put(const vector<int> & index, const T & input)
{
    if(index.size() < 1)
    {
        return false;
    }
    else
    {
        return Put(index[0], vector<int>(index.begin()+1, index.end()), input);
    }
}

template<class T>
bool Pyramid<T>::Trace(const PyramidDomain & pyramid_domain, const int source_level, const vector<int> & source_index, const int destination_level, vector<int> & destination_index) const
{
    if(LegalLevel(source_level) && LegalLevel(destination_level))
    {
        const vector<int> & source_size = _data[source_level]->Size();
        const vector<int> & destination_size = _data[destination_level]->Size();

        return pyramid_domain.Trace(source_size, source_index, destination_size, destination_index);
    }
    else
    {
        return false;
    }
}

template<class T>
bool Pyramid<T>::Trace(const PyramidDomain & pyramid_domain, const vector<int> & source, vector<int> & destination) const
{
    if((source.size() < 1) || (destination.size() < 1))
    {
        return false;
    }
    else
    {
        const int source_level = source[0];
        const vector<int> source_index(source.begin()+1, source.end());
        
        const int destination_level = destination[0];
        vector<int> destination_index(source_index);

        if(!Trace(pyramid_domain, source_level, source_index, destination_level, destination_index))
        {
            return false;
        }

        if(destination.size() != source.size())
        {
            destination = source;
        }

        destination[0] = destination_level;
        for(unsigned int i = 1; i < destination.size(); i++)
        {
            destination[i] = destination_index[i-1];
        }

        return true;
    }
}

template<class T>
vector<typename Pyramid<T>::ArrayPtr> Pyramid<T>::Construct(const vector< vector<int> > & level_sizes)
{
    vector<ArrayPtr> answer;

    for(unsigned int i = 0; i < level_sizes.size(); i++)
    {
        answer.push_back(new Array<T>(level_sizes[i]));
    }

    return answer;
}

template<class T>
vector< vector<int> > Pyramid<T>::DefaultLevelSizes(const vector<int> & top_level_size, const int num_levels)
{
    vector< vector<int> > answer;

    answer.push_back(top_level_size);
    for(int level = 1; level < num_levels; level++)
    {
        vector<int> level_size(answer[level-1]);
        
        for(unsigned int i = 0; i < level_size.size(); i++)
        {
            level_size[i] = (level_size[i]+1)/2;
        }

        answer.push_back(level_size);
    }

    return answer;
}

template<class T>
bool Pyramid<T>::LegalLevel(const int level) const
{
    return ((level >= 0) && (static_cast<unsigned int>(level) < _data.size()));
}

#endif
