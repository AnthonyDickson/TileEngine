add_library(LibEconSimPlusPlusEditor
        EconSimPlusPlus/Editor/Button.cpp
        EconSimPlusPlus/Editor/Editor.cpp
)
target_include_directories(LibEconSimPlusPlusEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(LibEconSimPlusPlusEditor PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow)

# Libraries
target_link_libraries(LibEconSimPlusPlusEditor PUBLIC LibEconSimPlusPlus)