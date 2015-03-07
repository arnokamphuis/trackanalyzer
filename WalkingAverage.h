#ifndef __WALKINGAVERAGE_H__
#define __WALKINGAVERAGE_H__

#include <iostream>

template<class T>
class WalkingAverage {
protected:
    int size;
    std::vector<T> values;
public:
    WalkingAverage(int s) : size(s) {
        assert(s>0);
    };

    void add(T v) {
        values.insert(values.begin(),v);
        if (values.size()>size)
            values.erase(values.end()-1);
    }

    inline T avg() const {
        T a = 0;
        for ( const auto& v : values ) a += v;
        return a / values.size();
    }

};

#endif // __WALKINGAVERAGE_H__