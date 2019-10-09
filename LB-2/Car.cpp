//
// Created by justas327 on 2019-10-07.
//

#include "Car.h"

void Car::fromJson(json data) {
    brand = data["brand"];
    makeYear = data["makeYear"];
    mileage = data["mileage"];
}

int Car::getNumber() {
    int number = ((makeYear + (int) mileage + (int) brand[0]) % 40);
    return fib(number);
}

string Car::toJson() {
    return "{\n"
           "'brand' : '" + brand + "',\n"
                                   "'makeYear' : " + to_string(makeYear) + ",\n"
                                                                           "'mileage' : " + to_string(mileage) +
           "\n"
           "},";
}

int Car::fib(int n) {
    if (n <= 1)
        return n;
    return fib(n - 1) + fib(n - 2);
}

const string &Car::getBrand() const {
    return brand;
}

int Car::getMakeYear() const {
    return makeYear;
}

double Car::getMileage() const {
    return mileage;
}
