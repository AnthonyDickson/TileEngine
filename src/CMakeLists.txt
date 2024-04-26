add_library(LibEconSimPlusPlus
        Camera.cpp
        Game.cpp
        InputState.cpp
        Shader.cpp
        Texture.cpp
        TileMap.cpp
        TileTypes.cpp
        Window.cpp
        VertexArray.cpp
        VertexBuffer.cpp
        stb_image.cpp
)
target_include_directories(LibEconSimPlusPlus PUBLIC ${PROJECT_SOURCE_DIR}/include/)
set_property(TARGET LibEconSimPlusPlus PROPERTY CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -Wpedantic -Wshadow -Wsign-conversion")


# Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(LibEconSimPlusPlus PUBLIC OpenGL::GL glad glfw glm stb_image yaml-cpp freetype)