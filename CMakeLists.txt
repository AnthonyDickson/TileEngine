cmake_minimum_required(VERSION 3.26)
project(EconSimPlusPlus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic")

add_executable(EconSimPlusPlus main.cpp)
