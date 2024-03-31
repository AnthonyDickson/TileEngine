cmake_minimum_required(VERSION 3.26)
project(EconSimPlusPlus)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Camera.cpp
        src/Shader.cpp
        src/Texture.cpp
        src/Window.cpp
        src/VertexArray.cpp
        src/VertexBuffer.cpp
        src/stb_image.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC include/)
set_property(TARGET ${PROJECT_NAME} PROPERTY CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic -Wshadow -Wsign-conversion")

# Libraries
## OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)
## Other libraries
add_subdirectory(lib/)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} stb_image)
