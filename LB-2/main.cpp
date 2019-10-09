#include <iostream>
#include "Car.h"
#include "nlohmann/json.hpp"
#include "Monitor.h"
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;
using json = nlohmann::json;
using namespace std::chrono;

#define DATA_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LB-2//data//IFF-7_2_MilisiunasJ_L1_dat_2.json"
#define RESULT_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LB-2//data//IFF-7_2_MilisiunasJ_L1_rez.txt"

#define FILTER_CONDITION 50000
#define NUMBER_OF_THREADS 2
#define WORK_ARRAY_SIZE 20

void filterCars(Monitor &work_list, Monitor &result_list);

void write_results_to_file(Monitor &cars, const string file_path, const string title);

vector<Car> readCarsFile(const string& filePath);

int main() {
    auto all_cars = readCarsFile(DATA_FILE_PATH);
    int n = all_cars.size();

    Monitor work(WORK_ARRAY_SIZE);
    Monitor results(n);

    milliseconds before = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
    );

#pragma omp parallel num_threads(NUMBER_OF_THREADS)
    {
        // Main thread
        if (omp_get_thread_num() == 0) {
            for (Car &car: all_cars) {
                work.add(car, false);
            }

            work.data_loading_finished();
        } else {
            // Other threads
            filterCars(work, results);
        }
    }

    milliseconds after = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
    );

    cout << "Uztruko: " << to_string((after.count() - before.count())) << " Dydis: " << results.get_size() << endl;

    remove(RESULT_FILE_PATH);
    write_results_to_file(results, RESULT_FILE_PATH, "Pradiniai duomenys:");
    write_results_to_file(results, RESULT_FILE_PATH, "Rezultatas:");
    return 0;
}

void filterCars(Monitor &work_list, Monitor &result_list) {
    while (!work_list.is_loading_finished() || work_list.get_size() != 0) {
        Car *current_car = work_list.pop();
        if (current_car == nullptr)
            continue;

        int filter_number = current_car->getNumber();
        if (filter_number < FILTER_CONDITION)
            result_list.add(*current_car, true);
    }
}

vector<Car> readCarsFile(const string& filePath) {
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

void write_results_to_file(Monitor &cars, const string file_path, const string title) {
    ofstream file;
    file.open(file_path, ios_base::app);
    file << setw(70) << title << endl
         << "------------------------------------------------------------------------------------------------------------------------"
         << endl
         << setw(30) << "Gamintojas |" << setw(30) << "Pagaminimo metai |" << setw(30) << "Rida |" << setw(30)
         << "Filtravimo numeris" << endl
         << "------------------------------------------------------------------------------------------------------------------------"
         << endl;
    for (int i = 0; i < cars.get_size(); ++i) {
        file << setw(30) << cars.get(i).getBrand() + " |" << setw(30) << to_string(cars.get(i).getMakeYear()) + " |"
             << setw(30)
             << to_string(cars.get(i).getMileage()) + " |" << setw(30) << to_string(cars.get(i).getNumber()) + " |"
             << endl;
    }

    file << endl << endl << endl;
}