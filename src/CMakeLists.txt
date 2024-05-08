add_library(LibEconSimPlusPlus
        Camera.cpp
        DeadReckoningAlgorithm.cpp
        Game.cpp
        Font.cpp
        FourSed.cpp
        FrameTimer.cpp
        Glyph.cpp
        InputState.cpp
        Shader.cpp
        Texture.cpp
        TextureArray.cpp
        TileMap.cpp
        Window.cpp
        VertexArray.cpp
        VertexBuffer.cpp
)
target_include_directories(LibEconSimPlusPlus PUBLIC ${PROJECT_SOURCE_DIR}/include/)
target_compile_options(LibEconSimPlusPlus PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow)


# Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(LibEconSimPlusPlus PUBLIC OpenGL::GL glad glfw glm stb_image yaml-cpp freetype)