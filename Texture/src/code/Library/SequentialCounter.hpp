/*
  SequentialCounter.hpp

  Li-Yi Wei
  07/07/2007

*/

#ifndef _SEQUENTIAL_COUNTER_HPP
#define _SEQUENTIAL_COUNTER_HPP

#include "Counter.hpp"

class SequentialCounter : public Counter
{
public:
    SequentialCounter(const int dimension, const int digit_min, const int digit_max);
    SequentialCounter(const int dimension, const vector<int> & digit_min, const vector<int> & digit_max);
    ~SequentialCounter(void);

    int Reset(void);

    int Get(vector<int> & value) const;
    
    int Next(void);

    // additional class specific functions
    int Reset(const int dimension, const int digit_min, const int digit_max);
    int Reset(const int dimension, const vector<int> & digit_min, const vector<int> & digit_max);

protected:
    vector<int> _digit_min;
    vector<int> _digit_max;
    vector<int> _value;
};

#endif
