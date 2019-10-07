//
// Created by justas327 on 2019-10-01.
//

#include <string>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>

#ifndef LD_1_SAFETHREADEDLIST_H
#define LD_1_SAFETHREADEDLIST_H

#define ITEMS_COUNT 100

using namespace std;

class SafeThreadedList {
private:
    unsigned int size;
    unsigned int capacity;
    shared_mutex m;
    Car *list;

    void push_array(int index) {
        for (int i = size; i >= index; i--) {
            list[i] = list[i - 1];
        }
    }

public:
    SafeThreadedList() {
        size = 0;
        capacity = ITEMS_COUNT;
        list = new Car[capacity];
    }

    explicit SafeThreadedList(int list_capacity) {
        capacity = list_capacity;
        size = 0;
        list = new Car[capacity];
    }

    ~SafeThreadedList() {
        delete[] list;
    }

    bool add(Car item, bool sort = false) {
        unique_lock lock(m);
        if (size > capacity) {
            return false;
        }

        if (!sort || size == 0) {
            list[size++] = item;
        } else {
            int item_number = item.getNumber();
            for (int i = 0; i < size; i++) {
                if (item_number < list[i].getNumber()) {
                    push_array(i);
                    list[i] = item;
                    size++;
                    break;
                } else if (i + 1 == size) {
                    list[size++] = item;
                    break;
                }
            }
        }
        return true;
    }

    bool addAll(vector<Car> &items) {
        for (const Car &item : items) {
            add(item);
        }
        return true;
    }

    Car *get(int index) {
        if (index > size)
            return nullptr;

        shared_lock lock(m);
        return &list[index];
    }

    [[nodiscard]] unsigned int getSize() const {
        return size;
    }
};


#endif //LD_1_SAFETHREADEDLIST_H
