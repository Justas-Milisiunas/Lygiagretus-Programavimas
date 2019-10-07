#include <iostream>
#include "Car.h"
#include "nlohmann/json.hpp"
#include "Monitor.h"

using namespace std;
using json = nlohmann::json;

#define DATA_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LB-1//data//IFF-7_2_MilisiunasJ_L1_dat_1.json"
#define RESULT_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LB-1//data//IFF-7_2_MilisiunasJ_L1_rez.txt"
#define FILTER_CONDITION 50000

void filterCars(Monitor& work_list, Monitor& result_list);

void write_results_to_file(Monitor& cars, const string file_path, const string title);

vector<Car> readCarsFile(string filePath);

int main() {
//    auto all_cars = readCarsFile(DATA_FILE_PATH);
//    int n = all_cars.size();

//    Monitor work(n);
//    Monitor results(n);

//    omp_set_num_threads(n/4);
//#pragma omp parallel private()
//    monitor.add(Car());
    return 0;
}
//
//void filterCars(Monitor& work_list, Monitor& result_list) {
//    for (int i = start_index; i < items_count; i++) {
//        if (cars.get(i)->getNumber() <= FILTER_CONDITION) {
//            filtered.add((*cars.get(i)), true);
//        }
//    }
//}
//
//vector<Car> readCarsFile(string filePath) {
//    vector<Car> cars;
//    ifstream stream(filePath);
//    json allCarsJson = json::parse(stream);
//    auto allCars = allCarsJson["cars"];
//    for (const json &new_car: allCars) {
//        Car tempCar;
//        tempCar.fromJson(new_car);
//        cars.push_back(tempCar);
//    }
//
//    return cars;
//}
//
//void write_results_to_file(SafeThreadedList &cars, const string file_path, const string title) {
//    ofstream file;
//    file.open(file_path, ios_base::app);
//    file << setw(70) << title << endl
//         << "------------------------------------------------------------------------------------------------------------------------"
//         << endl
//         << setw(30) << "Gamintojas |" << setw(30) << "Pagaminimo metai |" << setw(30) << "Rida |" << setw(30)
//         << "Filtravimo numeris" << endl
//         << "------------------------------------------------------------------------------------------------------------------------"
//         << endl;
//    for (int i = 0; i < cars.getSize(); ++i) {
//        file << setw(30) << cars.get(i)->getBrand() + " |" << setw(30) << to_string(cars.get(i)->getMakeYear()) + " |"
//             << setw(30)
//             << to_string(cars.get(i)->getMileage()) + " |" << setw(30) << to_string(cars.get(i)->getNumber()) + " |"
//             << endl;
//    }
//
//    file << endl << endl << endl;
//}