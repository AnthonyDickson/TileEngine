add_library(LibEconSimPlusPlusEditor
        Button.cpp
        Editor.cpp
)
target_include_directories(LibEconSimPlusPlusEditor PUBLIC ${PROJECT_SOURCE_DIR}/include/)
target_compile_options(LibEconSimPlusPlusEditor PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow)

# Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(LibEconSimPlusPlusEditor PUBLIC LibEconSimPlusPlus)