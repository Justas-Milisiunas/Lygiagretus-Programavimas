#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "Car.h"
#include "SafeThreadedList.h"
#include <thread>
#include <mutex>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;
using json = nlohmann::json;

#define DATA_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LB-1//data//IFF-7_2_MilisiunasJ_L1_dat_1.json"
#define RESULT_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LB-1//data//IFF-7_2_MilisiunasJ_L1_rez.txt"
#define FILTER_CONDITION 50000

void filterCars(SafeThreadedList &, int, int, SafeThreadedList &);

void write_results_to_file(SafeThreadedList &cars, const string file_path, const string title);

vector<Car> readCarsFile(string filePath);

void test(int sk);

int main() {
    auto allCars = readCarsFile(DATA_FILE_PATH);

    int n = allCars.size();
    SafeThreadedList filtered_cars = SafeThreadedList(n);

    auto cars = SafeThreadedList(n);
    cars.addAll(allCars);

    vector<thread> threads;
    unsigned int number_of_threads = thread::hardware_concurrency();
    threads.reserve(number_of_threads);

    unsigned int thread_array_size = (n + number_of_threads - 1) / number_of_threads;
    for (unsigned int i = 0; i < number_of_threads; i++) {
        unsigned int start_index = thread_array_size * i;
        unsigned int count = start_index + thread_array_size;
        if (count > n) {
            count = n;
        }

        threads.emplace_back(filterCars, ref(cars), start_index, count, ref(filtered_cars));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    remove(RESULT_FILE_PATH);
    write_results_to_file(ref(cars), RESULT_FILE_PATH, "Pradiniai duomenys");
    write_results_to_file(ref(filtered_cars), RESULT_FILE_PATH, "Atfiltruoti duomenis");

    cout << filtered_cars.getSize() << endl;
    return 0;
}

void filterCars(SafeThreadedList &cars, int start_index, int items_count, SafeThreadedList &filtered) {
    for (int i = start_index; i < items_count; i++) {
        if (cars.get(i)->getNumber() <= FILTER_CONDITION) {
            filtered.add((*cars.get(i)), true);
        }
    }
}

vector<Car> readCarsFile(string filePath) {
    vector<Car> cars;
    ifstream stream(filePath);
    json allCarsJson = json::parse(stream);
    auto allCars = allCarsJson["cars"];
    for (const json &new_car: allCars) {
        Car tempCar;
        tempCar.fromJson(new_car);
        cars.push_back(tempCar);
    }

    return cars;
}

void write_results_to_file(SafeThreadedList &cars, const string file_path, const string title) {
    ofstream file;
    file.open(file_path, ios_base::app);
    file << setw(70) << title << endl
         << "------------------------------------------------------------------------------------------------------------------------"
         << endl
         << setw(30) << "Gamintojas |" << setw(30) << "Pagaminimo metai |" << setw(30) << "Rida |" << setw(30)
         << "Filtravimo numeris" << endl
         << "------------------------------------------------------------------------------------------------------------------------"
         << endl;
    for (int i = 0; i < cars.getSize(); ++i) {
        file << setw(30) << cars.get(i)->getBrand() + " |" << setw(30) << to_string(cars.get(i)->getMakeYear()) + " |"
             << setw(30)
             << to_string(cars.get(i)->getMileage()) + " |" << setw(30) << to_string(cars.get(i)->getNumber()) + " |"
             << endl;
    }

    file << endl << endl << endl;
}