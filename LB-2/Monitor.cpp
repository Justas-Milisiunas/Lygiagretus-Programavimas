//
// Created by justas327 on 2019-10-07.
//

#include <iostream>
#include <wait.h>
#include <sys/wait.h>
#include "Monitor.h"

Monitor::Monitor(int size) {
    list = new Car[size];
    this->size = 0;
    this->capacity = size;
}

Monitor::~Monitor() {
    this->list = nullptr;
}

void Monitor::add(Car new_car) {
    while (this->size == capacity) {
        // wait
    }

    return new_car;
}

unsigned int Monitor::get_size() {
    return this->size;
}

int Monitor::find_index(Car car) {
    int condition = car.getNumber();

    for (int i = 0; i < size; i++) {
        if (condition < list[i].getNumber())
            return i;
    }

    return -1;
}

void Monitor::shift_list(int index) {
    for(int i = size - 1; i > index; i--) {
        list[i] = list[i-1];
    }
}

void Monitor::get(unsigned int index) {

}
