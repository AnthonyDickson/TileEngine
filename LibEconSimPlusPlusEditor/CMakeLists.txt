add_library(LibEconSimPlusPlusEditor
        EconSimPlusPlus/Editor/Editor.cpp
        EconSimPlusPlus/Editor/MessageDialog.cpp
        EconSimPlusPlus/Editor/OpenFileDialog.cpp
        EconSimPlusPlus/Editor/SaveFileDialog.cpp
)
target_include_directories(LibEconSimPlusPlusEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(LibEconSimPlusPlusEditor PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow)

# Libraries
target_link_libraries(LibEconSimPlusPlusEditor PUBLIC LibEconSimPlusPlus)
target_link_libraries(LibEconSimPlusPlusEditor PUBLIC portable_file_dialogs)