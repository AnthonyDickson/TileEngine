cmake_minimum_required(VERSION 3.26)
project(EconSimPlusPlus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic")

add_executable(${PROJECT_NAME} src/main.cpp
        src/shader.cpp)

include_directories(include/)

# GLFW
add_subdirectory(glfw)
target_link_libraries(${PROJECT_NAME} glfw)
include_directories(glfw/include)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# GLAD
add_library(glad glad/include/glad/glad.h glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)
target_link_libraries(${PROJECT_NAME} glad)
include_directories(glad/include)