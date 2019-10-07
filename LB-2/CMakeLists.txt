cmake_minimum_required(VERSION 3.14)
project(LB_2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
find_package(OpenMP)

add_executable(LB_2 main.cpp Monitor.cpp Monitor.h Car.cpp Car.h)