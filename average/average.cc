#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>

#include "average.h"

using namespace std;

bool sum_breachs_range(float val1, float val2, float min, float max) {
    if ((val1 >= 0) ^ (val2 >= 0)) {
        return false;
    }

    return val1 > 0 && val1 > max - val2 || val1 < min - val2;
}


Average::Average(float min, float max)
            :min(min), max(max) { }

void Average::add_value(float val) {
    if (val < min || val > max) {
        cout<<"invalid value "<<val<<"!!"<<endl;
        return;
    }

    if (buckets.empty()) {
        buckets.push_back(new Bucket());
    }

    auto current_bucket = buckets.back();
    if (!current_bucket->add_value(val, min, max)) {
        auto new_bucket = new Bucket();
        new_bucket->add_value(val, min, max);
        buckets.push_back(new_bucket);
    }
}

float Average::get() const {
    return calculate_average();
}

/**
 * @brief reduce number of buckets by merging buckets, 
 * additionally, eliminating buckets of everage devivation from overall everage.
 */

void Average::reconcile_buckets() const {
    if (buckets.size() < 2) {
        return;
    }
    
    cout<<"reconcile_buckets start - buckets.size() "<<buckets.size()<<endl;

    sort(buckets.begin(), buckets.end(), [](const Bucket* b1, const Bucket* b2) -> bool {
            return b1->sum > b2->sum;
        });

    bool reconciled_last_turn = true;
    while (reconciled_last_turn) {

        size_t size_beforehand = buckets.size();
        
        auto iter = buckets.begin();
        auto iter_r = buckets.rbegin();
        while (*iter != *iter_r) {
            auto bucket = (*iter);
            auto bucket_r = (*iter_r);
            if (!sum_breachs_range(bucket->sum, bucket_r->sum, min, max)) {
                bucket->sum += bucket_r->sum;
                bucket->count += bucket_r->count;

                bucket_r->sum = 0;
                bucket_r->count = 0;
                
                ++iter_r;
            } else {
                ++iter;
            }
        }

        // clear empty buckets after reconcilation.
        buckets.erase(remove_if(buckets.begin(), buckets.end(), [](const Bucket* bucket) -> bool {
            if (bucket->sum == 0) {
                delete bucket;
                return true;
            } else {
                return false;
            }
        }), buckets.end());

        reconciled_last_turn = (buckets.size() < size_beforehand);
    }
    
    cout<<"reconcile_buckets end - buckets.size() "<<buckets.size()<<endl;
}


float Average::calculate_average() const {
    
    reconcile_buckets();

    float estimated_average = estimate_average();
    if (buckets.size() < 2) {
        return estimated_average;
    }

    float overhead_sum = 0;
    int all_count = 0;

    // cout<<"calculate_average - estimated_average:"<<estimated_average<<endl;

    for (auto& bucket: buckets) {
        // employ double to replace this block?
        auto increment_overhead = bucket->sum;
        for (auto i=0; i<bucket->count; ++i) {
            increment_overhead -= estimated_average;
        }

        overhead_sum += increment_overhead;
        all_count += bucket->count;
    }

    // cout<<"calculate_average:"<<estimated_average + overhead_sum / all_count<<endl;

    return estimated_average + overhead_sum / all_count;
}

float Average::estimate_average() const {
    float accumulating_average = 0;
    int all_count = 0;

    cout<<"estimate_average - start"<<endl;

    bool isFirst = true;
    for (auto& bucket: buckets) {
        if (isFirst) {
            accumulating_average = bucket->average();
            cout<<"estimate_average - accumulating_average:"<<accumulating_average<<endl;
            all_count += bucket->count;
            isFirst = false;
            continue;
        }
        auto average_gap = bucket->average() - accumulating_average;
        auto overhead_sum = average_gap * bucket->count;
        all_count += bucket->count;
        auto average_increment = overhead_sum / all_count;
        accumulating_average += average_increment;
    }

    cout<<"estimate_average - accumulating_average:"<<accumulating_average<<endl;

    return accumulating_average;
}
