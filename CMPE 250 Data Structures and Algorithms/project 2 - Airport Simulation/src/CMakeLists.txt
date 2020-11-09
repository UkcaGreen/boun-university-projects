cmake_minimum_required(VERSION 2.6)
project(Project_2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Event.cpp Event.h Passenger.cpp Passenger.h EventQueue.cpp EventQueue.h)
add_executable(project2 ${SOURCE_FILES})