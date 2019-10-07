//
// Created by justas327 on 2019-10-07.
//


#ifndef LB_2_MONITOR_H
#define LB_2_MONITOR_H


#include <mutex>
#include "Car.h"
#include <condition_variable>

using namespace std;

class Monitor {
private:
    int find_index(Car car);
    void shift_list(int index);
    unsigned int size;
    unsigned int capacity;
    mutex m;
    condition_variable cv;
    Car *list;
public:
    Monitor(int size);
    ~Monitor();
    void add(Car new_car);
    void get(unsigned int index);
    unsigned int get_size();
};


#endif //LB_2_MONITOR_H
