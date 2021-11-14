#ifndef AVERAGE_H
#define AVERAGE_H

#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>

#include <stdio.h>

using namespace std;

bool sum_breachs_range(float val1, float val2, float min, float max);

struct Bucket {
    float sum;
    int count;

    bool add_value(float val, float min, float max) {
        if (sum_breachs_range(sum, val, min, max)) {
            return false;
        }
        
        sum += val;
        ++count;

        return true;
    }

    float average() const {
        if (count == 0) {
            return 0;
        }
        return sum / (float)count;
    }
};

class Average {

public:
    Average(float min = -numeric_limits<float>::max(), 
            float max = numeric_limits<float>::max());

    void add_value(float val);

    float get() const;


private:
    const float min;
    const float max;

    mutable vector<Bucket*> buckets;

    void reconcile_buckets() const;

    float calculate_average() const;

    float estimate_average() const;
};

#endif /* AVERAGE_H */