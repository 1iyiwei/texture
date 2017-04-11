/*
  SequentialCounter.cpp

  Li-Yi Wei
  07/07/2007
  
*/

#include "SequentialCounter.hpp"

SequentialCounter::SequentialCounter(const int dimension, const int digit_min, const int digit_max) : Counter(dimension), _digit_min(dimension), _digit_max(dimension), _value(dimension)
{
    for(int i = 0; i < dimension; i++)
    {
        _digit_min[i] = digit_min;
        _digit_max[i] = digit_max;
    }
    
    Reset();
}

SequentialCounter::SequentialCounter(const int dimension, const vector<int> & digit_min, const vector<int> & digit_max) : Counter(dimension), _digit_min(digit_min), _digit_max(digit_max), _value(dimension)
{
    Reset();
}

SequentialCounter::~SequentialCounter(void)
{
    // nothing to do
}

int SequentialCounter::Reset(void)
{
    for(unsigned int i = 0; i < _value.size(); i++)
    {
        _value[i] = _digit_min[i];
    }

    return 1;
}

int SequentialCounter::Get(vector<int> & value) const
{
    value = _value;

    return 1;
}

int SequentialCounter::Next(void)
{
    unsigned int which_dim = 0;
    
    for(which_dim = 0; which_dim < _value.size(); which_dim++)
    {
        if(_value[which_dim] < _digit_max[which_dim])
        {
            _value[which_dim]++;

            for(unsigned int i = 0; i < which_dim; i++)
            {
                _value[i] = _digit_min[i];
            }

            break;
        }
    }

    return (which_dim < _value.size());
}

int SequentialCounter::Reset(const int dimension, const int digit_min, const int digit_max)
{
    return Reset(dimension, vector<int>(dimension, digit_min), vector<int>(dimension, digit_max));
}

int SequentialCounter::Reset(const int dimension, const vector<int> & digit_min, const vector<int> & digit_max)
{
    if(dimension != Counter::Dimension())
    {
        return 0;
    }
    else
    {
        _digit_min = digit_min;
        _digit_max = digit_max;
        return Reset();
    }
}
