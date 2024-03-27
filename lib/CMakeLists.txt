include(FetchContent)

# glfw
FetchContent_Declare(
        glfw
        URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
        URL_MD5 7d1e90c5c0011eb10eac280b6093d0d0
)
FetchContent_MakeAvailable(glfw)