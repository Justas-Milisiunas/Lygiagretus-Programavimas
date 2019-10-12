//
// Created by justas327 on 2019-10-10.
//

#ifndef LD_1A_CAR_H
#define LD_1A_CAR_H

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
    Car();

    void fromJson(json data);

    int getNumber();

    string toJson();

    [[nodiscard]] const string &getBrand() const;

    [[nodiscard]] int getMakeYear() const;

    [[nodiscard]] double getMileage() const;
};


#endif //LD_1A_CAR_H
