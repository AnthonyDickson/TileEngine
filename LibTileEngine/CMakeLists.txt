add_library(LibTileEngine
        TileEngine/Anchor.cpp
        TileEngine/Button.cpp
        TileEngine/Camera.cpp
        TileEngine/Font.cpp
        TileEngine/FrameTimer.cpp
        TileEngine/Glyph.cpp
        TileEngine/GridLines.cpp
        TileEngine/Group.cpp
        TileEngine/Image.cpp
        TileEngine/InputState.cpp
        TileEngine/KeyModifier.cpp
        TileEngine/Object.cpp
        TileEngine/Outline.cpp
        TileEngine/Quad.cpp
        TileEngine/Shader.cpp
        TileEngine/SignedDistanceField.cpp
        TileEngine/Text.cpp
        TileEngine/TextCaret.cpp
        TileEngine/TextField.cpp
        TileEngine/Texture.cpp
        TileEngine/TextureArray.cpp
        TileEngine/TileMap.cpp
        TileEngine/TileSheet.cpp
        TileEngine/TwoColumnLayout.cpp
        TileEngine/Window.cpp
        TileEngine/VertexArray.cpp
        TileEngine/VertexBuffer.cpp
)
target_include_directories(LibTileEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(LibTileEngine PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow)


# Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(LibTileEngine PUBLIC OpenGL::GL glad glfw glm stb_image yaml-cpp freetype)