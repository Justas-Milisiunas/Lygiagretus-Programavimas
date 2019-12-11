#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "nlohmann/json.hpp"
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>

using namespace std;
using json = nlohmann::json;
using namespace thrust;

const int MAX_STRING_LENGTH = 256;
#define DATA_FILE "//home//justmili//data//IFF-7_2_MilisiunasJ_L1_dat_1.json"
#define RESULTS_FILE "//home//justmili//data//IFF-7_2_MilisiunasJ_L3_results_b.json"

struct Car {
    char brand[MAX_STRING_LENGTH];
    int make_year;
    double mileage;

    void from_json(json data) {
        string brand_temp = data["brand"];
        memset(brand, 0, sizeof(brand));
        strcpy(brand, brand_temp.c_str());

        make_year = data["makeYear"];
        mileage = data["mileage"];
    }
};


host_vector<Car> reads_cars_file(const string file_path);
void write_results_to_file(Car result, const string file_path, const string title);

__device__ void gpu_strcat(char* dest, char* src);
__device__ void gpu_memset(char* dest);

/**
 * Function for reducing cars vector
 */
struct sum_func {
    __device__ Car operator()(Car accumulator, Car item) {
        gpu_strcat(accumulator.brand, item.brand);
        accumulator.make_year += item.make_year;
        accumulator.mileage += item.mileage;
        return accumulator;
    }
};

int main() {
    host_vector<Car> host_cars = reads_cars_file(DATA_FILE);
    device_vector<Car> device_cars = host_cars;

    device_vector<Car> filtered_cars(device_cars.size());
    Car sum;
    memset(sum.brand, 0, sizeof(sum.brand));
    sum.make_year = 0;
    sum.mileage = 0.0;

    Car result = reduce(device_cars.begin(), device_cars.end(), sum, sum_func());

    printf("Calculations finished!\n");
    printf("Brand: %s Make Year: %d Mileage: %f\n", result.brand, result.make_year, result.mileage);
    write_results_to_file(result, RESULTS_FILE, "B dalies rezultatai");
    return 0;
}

/**
 * Appends char array to other char array
 * @param dest Destination array
 * @param src Source array
 */
__device__ void gpu_strcat(char* dest, char* src) {
    for (int i = 0; i < MAX_STRING_LENGTH; ++i) {
        if (dest[i] == 0) {
            for (int j = 0; j < MAX_STRING_LENGTH; ++j) {
                if (src[j] != 0) {
                    dest[i + j] = src[j];
                }
            }
            break;
        }
    }
}

/**
 * Zeroes all char memory
 * @param dest Char array
 */
__device__ void gpu_memset(char* dest) {
    for (int i = 0; i < MAX_STRING_LENGTH; ++i) {
        dest[i] = 0;
    }
}

/**
 * Reads cars data in json format from the file,
 * parses it
 * @param file_path Data file path
 */
host_vector<Car> reads_cars_file(const string file_path) {
    host_vector<Car> cars;

    ifstream stream(DATA_FILE);
    json allCarsJson = json::parse(stream);
    auto allCars = allCarsJson["cars"];

    for (const json &new_car: allCars) {
        Car tempCar;
        tempCar.from_json(new_car);
        cars.push_back(tempCar);
    }

    return cars;
}

/**
 * Writes given monitor cars formatted in table to file
 * @param cars Cars list
 * @param file_path Result file path
 * @param title Results table title
 */
void write_results_to_file(Car result, const string file_path, const string title) {
    ofstream file;
    file.open(file_path, ios_base::app);
    string str(result.brand);

    file << setw(70) << title << endl;
    file << "Gamintojas: " << str << endl;
    file << "Pagaminimo metai: " << to_string(result.make_year) << endl;
    file << "Rida: " << to_string(result.mileage) << endl;
}