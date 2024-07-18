add_library(LibEconSimPlusPlus
        EconSimPlusPlus/Anchor.cpp
        EconSimPlusPlus/Button.cpp
        EconSimPlusPlus/Camera.cpp
        EconSimPlusPlus/Font.cpp
        EconSimPlusPlus/FrameTimer.cpp
        EconSimPlusPlus/Glyph.cpp
        EconSimPlusPlus/GridLines.cpp
        EconSimPlusPlus/Group.cpp
        EconSimPlusPlus/Image.cpp
        EconSimPlusPlus/InputState.cpp
        EconSimPlusPlus/KeyModifier.cpp
        EconSimPlusPlus/Object.cpp
        EconSimPlusPlus/Outline.cpp
        EconSimPlusPlus/Quad.cpp
        EconSimPlusPlus/Shader.cpp
        EconSimPlusPlus/SignedDistanceField.cpp
        EconSimPlusPlus/Text.cpp
        EconSimPlusPlus/TextCaret.cpp
        EconSimPlusPlus/TextField.cpp
        EconSimPlusPlus/Texture.cpp
        EconSimPlusPlus/TextureArray.cpp
        EconSimPlusPlus/TileMap.cpp
        EconSimPlusPlus/TileSheet.cpp
        EconSimPlusPlus/TwoColumnLayout.cpp
        EconSimPlusPlus/Window.cpp
        EconSimPlusPlus/VertexArray.cpp
        EconSimPlusPlus/VertexBuffer.cpp
)
target_include_directories(LibEconSimPlusPlus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(LibEconSimPlusPlus PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow)


# Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(LibEconSimPlusPlus PUBLIC OpenGL::GL glad glfw glm stb_image yaml-cpp freetype)