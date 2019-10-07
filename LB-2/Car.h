//
// Created by justas327 on 2019-10-07.
//

#ifndef LB_2_CAR_H
#define LB_2_CAR_H

#include "nlohmann/json.hpp"
#include <string>

using namespace std;
using json = nlohmann::json;

class Car {
private:
    string brand;
    int makeYear;
    double mileage;

    static int fib(int n);

public:
    void fromJson(json data);

    int getNumber();

    string toJson();

    const string &getBrand() const;

    int getMakeYear() const;

    double getMileage() const;
};


#endif //LB_2_CAR_H
