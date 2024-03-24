cmake_minimum_required(VERSION 3.26)
project(EconSimPlusPlus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic")

file(GLOB_RECURSE PROJECT_SOURCE src/*.cpp)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE})

include_directories(include/)

# stb_image
include_directories(lib/stb_image/include)

# GLFW
add_subdirectory(lib/glfw)
target_link_libraries(${PROJECT_NAME} glfw)
include_directories(lib/glfw/include)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

# GLAD
add_library(glad lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)
target_link_libraries(${PROJECT_NAME} glad)
include_directories(lib/glad/include)

# GLM
include_directories(lib/glm)