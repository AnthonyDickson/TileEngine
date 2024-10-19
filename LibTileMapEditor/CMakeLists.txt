add_library(LibTileMapEditor
        TileEngine/Editor/MessageDialog.cpp
        TileEngine/Editor/OpenFileDialog.cpp
        TileEngine/Editor/SaveFileDialog.cpp
)
target_include_directories(LibTileMapEditor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(LibTileMapEditor PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow)

# Libraries
target_link_libraries(LibTileMapEditor PUBLIC LibTileEngine)
target_link_libraries(LibTileMapEditor PUBLIC portable_file_dialogs)