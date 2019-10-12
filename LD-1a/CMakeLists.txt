cmake_minimum_required(VERSION 3.14)
project(LD_1a)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(LD_1a main.cpp Monitor.cpp Monitor.h Car.cpp Car.h)