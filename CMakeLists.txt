cmake_minimum_required(VERSION 3.26)
project(EconSimPlusPlus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic")

add_executable(EconSimPlusPlus main.cpp)

# GLFW
add_subdirectory(glfw)
target_link_libraries(EconSimPlusPlus glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(EconSimPlusPlus OpenGL::GL)