cmake_minimum_required(VERSION 3.10)
project(untitled5)

set(CMAKE_CXX_STANDARD 17)
set(BOOST_ROOT "/home/vover/prog/boost_1_70_0")
find_package(Boost 1.70.0)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()
set(CMAKE_CXX_STANDARD_LIBRARIES  "-lOpenCL")

add_executable(untitled5 main.cpp ReedSolomon.cpp ReedSolomon.h)