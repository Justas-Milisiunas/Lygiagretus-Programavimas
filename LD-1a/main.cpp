#include <iostream>
#include <fstream>
#include "Monitor.h"
#include <string>
#include "Car.h"
#include <iomanip>
#include <thread>

using namespace std;
using json = nlohmann::json;
using namespace std::chrono;

#define DATA_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LD-1a//data//IFF-7_2_MilisiunasJ_L1_dat_2.json"
#define RESULT_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LD-1a//data//IFF-7_2_MilisiunasJ_L1_rez.txt"

#define FILTER_CONDITION 50000
#define NUMBER_OF_THREADS 20
#define WORK_ARRAY_SIZE 10

void filterCars(Monitor &work_list, Monitor &result_list);

void write_results_to_file(Monitor &cars, const string file_path, const string title);

void write_results_to_file(vector<Car> &cars, const string file_path, const string title);

vector<Car> readCarsFile(const string &filePath);

int main() {
    // Reads cars data to vector
    auto all_cars = readCarsFile(DATA_FILE_PATH);
    int n = all_cars.size();

    // Initializes work and results monitors
    Monitor work(WORK_ARRAY_SIZE);
    Monitor results(n);

    // Current time in ms
    milliseconds before = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
    );

    vector<thread> threads;
    threads.reserve(NUMBER_OF_THREADS);

    // Starts threads
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        threads.emplace_back(filterCars, ref(work), ref(results));
    }

    // Starts adding cars to work monitor
    for (int i = 0; i < all_cars.size(); i++) {
        work.add(all_cars[i]);
    }

    // Sets flag - no more data will be added
    work.data_loading_finished();


    // Waits for all threads to finish
    for (auto &thread: threads) {
        thread.join();
    }

    // Current time in ms
    milliseconds after = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
    );

    // Shows filtering performance measured in ms
    cout << "Uztruko: " << to_string((after.count() - before.count())) << " Dydis: " << results.get_size() << endl;

    // Write primary data and filtered data to file
    remove(RESULT_FILE_PATH);
    write_results_to_file(all_cars, RESULT_FILE_PATH, "Pradiniai duomenys:");
    write_results_to_file(results, RESULT_FILE_PATH, "Rezultatas:");
    return 0;
}

/**
 * Filters work monitor car objects
 * @param work_list  Work monitor
 * @param result_list  Filtered cars monitor
 */
void filterCars(Monitor &work_list, Monitor &result_list) {
    while (!work_list.is_loading_finished() || work_list.get_size() != 0) {
        Car current_car = work_list.pop();
//        if (current_car == nullptr)
//            continue;

        if (current_car.getBrand().empty())
            continue;

        int filter_number = current_car.getNumber();
        if (filter_number < FILTER_CONDITION) {
            result_list.add(current_car, true);
        }
    }
}

/**
 * Reads json data from file, serializes to Car objects and adds to vector
 * @param filePath Data file path
 * @return Vector with car objects
 */
vector<Car> readCarsFile(const string &filePath) {
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

/**
 * Writes given monitor cars formatted in table to file
 * @param cars Cars monitor
 * @param file_path Result file path
 * @param title Results table title
 */
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

/**
 * Writes given monitor cars formatted in table to file
 * @param cars Cars list
 * @param file_path Result file path
 * @param title Results table title
 */
void write_results_to_file(vector<Car> &cars, const string file_path, const string title) {
    ofstream file;
    file.open(file_path, ios_base::app);
    file << setw(70) << title << endl
         << "------------------------------------------------------------------------------------------------------------------------"
         << endl
         << setw(30) << "Gamintojas |" << setw(30) << "Pagaminimo metai |" << setw(30) << "Rida |" << setw(30)
         << "Filtravimo numeris" << endl
         << "------------------------------------------------------------------------------------------------------------------------"
         << endl;
    for (int i = 0; i < cars.size(); ++i) {
        file << setw(30) << cars[i].getBrand() + " |" << setw(30) << to_string(cars[i].getMakeYear()) + " |"
             << setw(30)
             << to_string(cars[i].getMileage()) + " |" << setw(30) << to_string(cars[i].getNumber()) + " |"
             << endl;
    }

    file << endl << endl << endl;
}