#include "cuda_runtime.h"
#include <cuda.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include "device_launch_parameters.h"

using namespace std;
using json = nlohmann::json;

#define MAX_STRING_LENGTH 255

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

void readCarsFile(vector<Car> *cars);
__global__ void sum_on_gpu(Car* cars, int* n, int* chunk_size, Car* results);
__device__ void gpu_memset(char* dest);
__device__ void gpu_strcat(char* dest, char* src);

int main() {
    int threads_count = 10;
    vector<Car> all_cars;
    readCarsFile(&all_cars);

    // Host
    Car* cars = &all_cars[0];
    int n = all_cars.size();
    int chunk_size = n / threads_count;

    // GPU
    Car* d_all_cars;
    int* d_n;
    int* d_chunk_size;
    Car* d_results;

    cout << cars[0].brand;

    // Memory allocation for GPU
    cudaMalloc((void**)&d_all_cars, n * sizeof(Car));
    cudaMalloc((void**)&d_results, threads_count);
    cudaMalloc((void**)&d_n, sizeof(int));
    cudaMalloc((void**)&d_chunk_size, sizeof(int));

    // Copies memory from CPU to GPU
    cudaMemcpy(d_all_cars, cars, n * sizeof(Car), cudaMemcpyHostToDevice);
    cudaMemcpy(d_n, &n, sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_chunk_size, &chunk_size, sizeof(int), cudaMemcpyHostToDevice);

    sum_on_gpu<<<1,threads_count>>>(d_all_cars, d_n, d_chunk_size, d_results);
    cudaDeviceSynchronize();
    cout << "Finished" << endl;
    return 0;
}

__global__ void sum_on_gpu(Car* cars, int* n, int* chunk_size, Car* results) {
    int start_index = threadIdx.x * *chunk_size;
    int end_index = min(start_index + 5, *n);

    if (end_index + *chunk_size >= *n) {
        end_index = *n;
    }

    Car sum;
    gpu_memset(sum.brand);
    sum.make_year = 0;
    sum.mileage = 0.0;

    for (int i = start_index; i < end_index; ++i) {
        if (i < *n) {
            gpu_strcat(sum.brand, cars[i].brand);
            sum.make_year += cars[i].make_year;
            sum.mileage += cars[i].mileage;
        }
    }

    printf("Thread: %d Start Index: %d End Index: %d\n", threadIdx.x, start_index, end_index);
    results[threadIdx.x] = sum;
    printf("RESULT Brand: %s Make Year: %d Mileage: %f\n", results[threadIdx.x].brand, results[threadIdx.x].make_year, results[threadIdx.x].mileage);
}

__device__ void gpu_strcat(char* dest, char* src) {
    for (int i = 0; i < 256; ++i) {
        if (dest[i] == 0) {
            for (int j = 0; j < 256; ++j) {
                if (src[j] != 0) {
                    dest[i + j] = src[j];
                }
            }
            break;
        }
    }
}

__device__ void gpu_memset(char* dest) {
    for (int i = 0; i < 256; ++i) {
        dest[i] = 0;
    }
}

void readCarsFile(vector<Car> *cars) {
    ifstream stream(
            "//home//justmili//data//IFF-7_2_MilisiunasJ_L1_dat_1.json");
    json allCarsJson = json::parse(stream);
    auto allCars = allCarsJson["cars"];
    for (const json &new_car: allCars) {
        Car tempCar;
        tempCar.from_json(new_car);
        cars->push_back(tempCar);
    }
}