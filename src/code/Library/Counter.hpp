/*
  Counter.hpp

  the base class for all kinds of counters

  Li-Yi Wei

*/

#ifndef _COUNTER_HPP
#define _COUNTER_HPP

#include <vector>
using namespace std;

class Counter
{
public:
    Counter(const int dimension);
    virtual ~Counter(void) = 0;

    // return 1 if successful, 0 else
    virtual int Reset(void);

    // get the current counter value
    // return 1 if successful, 0 else
    virtual int Get(vector<int> & value) const;
    
    // move to the next value
    // return 1 if successful, 0 if the counter stopped (already in final value)
    virtual int Next(void);
    
    int Dimension(void) const;
    
protected:
    struct Sortee
    {
        int operator<(const Sortee & another) const;
        vector<int> value;
        int key; // for sorting
    };

protected:
    const int _dimension;
};

#endif
