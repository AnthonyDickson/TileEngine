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