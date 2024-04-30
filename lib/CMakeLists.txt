include(FetchContent)

# glfw
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# GLAD
add_library(glad glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)

## stb_image
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE stb_image/include)

# YAML-CPP
FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 0.8.0
        SYSTEM # This seems to get rid of compiler warning from this dependency.
)
FetchContent_MakeAvailable(yaml-cpp)

# TODO: Make sure freetype uses this harfbuzz instead of system harfbuzz.
# HarfBuzz, a dependency of FreeType.
FetchContent_Declare(
        harfbuzz
        GIT_REPOSITORY https://github.com/harfbuzz/harfbuzz.git
        GIT_TAG 8.4.0
)
FetchContent_MakeAvailable(harfbuzz)
# FreeType Fonts
add_subdirectory(freetype)