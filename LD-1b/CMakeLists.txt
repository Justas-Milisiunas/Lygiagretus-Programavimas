cmake_minimum_required(VERSION 3.14)
project(LD_1b)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
find_package(OpenMP)

add_executable(LD_1b main.cpp Monitor.cpp Monitor.h Car.cpp Car.h)