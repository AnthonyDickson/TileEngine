add_library(LibEconSimPlusPlus
        EconSimPlusPlus/Engine/Camera.cpp
        EconSimPlusPlus/Engine/Font.cpp
        EconSimPlusPlus/Engine/FrameTimer.cpp
        EconSimPlusPlus/Engine/Game.cpp
        EconSimPlusPlus/Engine/GameObject.cpp
        EconSimPlusPlus/Engine/Glyph.cpp
        EconSimPlusPlus/Engine/GridLines.cpp
        EconSimPlusPlus/Engine/InputState.cpp
        EconSimPlusPlus/Engine/Shader.cpp
        EconSimPlusPlus/Engine/SignedDistanceField.cpp
        EconSimPlusPlus/Engine/Texture.cpp
        EconSimPlusPlus/Engine/TextureArray.cpp
        EconSimPlusPlus/Engine/TileMap.cpp
        EconSimPlusPlus/Engine/Window.cpp
        EconSimPlusPlus/Engine/VertexArray.cpp
        EconSimPlusPlus/Engine/VertexBuffer.cpp
)
target_include_directories(LibEconSimPlusPlus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(LibEconSimPlusPlus PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow)


# Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(LibEconSimPlusPlus PUBLIC OpenGL::GL glad glfw glm stb_image yaml-cpp freetype)