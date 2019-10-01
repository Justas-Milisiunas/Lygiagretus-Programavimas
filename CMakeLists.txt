cmake_minimum_required(VERSION 3.14)
project(LD_1)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(LD_1 main.cpp Car.cpp Car.h SafeThreadedList.h)