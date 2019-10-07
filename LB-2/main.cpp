#include <iostream>
#include "Car.h"
#include "nlohmann/json.hpp"
#include "Monitor.h"

using namespace std;
using json = nlohmann::json;

#define DATA_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LB-1//data//IFF-7_2_MilisiunasJ_L1_dat_1.json"
#define RESULT_FILE_PATH "//home//justas327//Github//Lygiagretus-Programavimas//LB-1//data//IFF-7_2_MilisiunasJ_L1_rez.txt"
#define FILTER_CONDITION 50000

//void filterCars(SafeThreadedList &, int, int, SafeThreadedList &);

//void write_results_to_file(SafeThreadedList &cars, const string file_path, const string title);

vector<Car> readCarsFile(string filePath);

int main() {
    std::cout << "Hello, World!" << std::endl;
    Monitor monitor(100);
    monitor.add(Car());
    return 0;
}