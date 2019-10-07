#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-narrowing-conversions"
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
    this->finished = false;
}

Monitor::~Monitor() {
    delete[] list;
}

void Monitor::add(Car& new_car) {
    omp_set_lock(&lock);
    if (this->size == capacity || finished) {
        omp_unset_lock(&lock);
        return;
    }

    unsigned int index = find_index(new_car);
    if(index != -1) {
        shift_list(index);
        list[index] = new_car;
        size++;
    }

    omp_unset_lock(&lock);
}

unsigned int Monitor::get_size() {
    return this->size;
}

int Monitor::find_index(Car& car) {
    int condition = car.getNumber();

    for (unsigned int i = 0; i < size; i++) {
        if (condition < list[i].getNumber())
            return i;
    }

    return -1;
}

void Monitor::shift_list(int index) {
    for (int i = size - 1; i > index; i--) {
        list[i] = list[i - 1];
    }
}

Car Monitor::get(unsigned int index) {
    omp_set_lock(&lock);
    if(size == 0) {
        omp_unset_lock(&lock);
        return Car();
    }

    Car foundCar = list[index];
    omp_unset_lock(&lock);
    return foundCar;
}

void Monitor::data_loading_finished() {
    this->finished = true;
}

bool Monitor::is_loading_finished() {
    return this->finished;
}

#pragma clang diagnostic pop