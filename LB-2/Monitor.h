//
// Created by justas327 on 2019-10-07.
//


#ifndef LB_2_MONITOR_H
#define LB_2_MONITOR_H


#include <mutex>
#include "Car.h"
#include <condition_variable>
#include <omp.h>

using namespace std;

class Monitor {
private:
    int find_index(Car& car);

    void shift_list(int index);

    unsigned int size;
    unsigned int capacity;

    omp_lock_t lock;
    condition_variable cv;

    bool finished;
    Car *list;
public:
    Monitor(int size);

    ~Monitor();

    void add(Car& new_car);

    Car get(unsigned int index);

    unsigned int get_size();

    void data_loading_finished();
    bool is_loading_finished();
};


#endif //LB_2_MONITOR_H
