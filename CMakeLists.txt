cmake_minimum_required(VERSION 3.26)
project(EconSimPlusPlus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

add_subdirectory(lib)
add_subdirectory(LibEconSimPlusPlus)
add_subdirectory(LibEconSimPlusPlusEditor)
add_subdirectory(Game)
add_subdirectory(Editor)
