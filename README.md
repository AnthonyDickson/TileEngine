# TileEngine

This project contains the beginnings of a 2D game engine and a tile map editor
for [tile-based video games](https://en.wikipedia.org/wiki/Tile-based_video_game).
Custom GUI and font rendering frameworks are developed.

## Build and Run

You can build the application with the following commands:

```shell
mkdir build && cd build
cmake ..
make -j8
cd ..
```

To run the tile map editor, run:

```shell
./build/Editor/TileMapEditor
```

To run the game engine, run:

```shell
./build/Game/TileEngine
```

This project also includes run configs for the CLion IDE.

This project has only been built and tested on Linux and macOS.

## Project Structure

- [Editor/](./Editor) contains the source files for the editor executable.
- [Game/](./Game) contains the source files for the game executable.
- [LibTileEngine/](./LibTileEngine) contains the source files for the engine.
- [LibTileMapEditor](./LibTileMapEditor) contains the source files specific to the editor.
- [resource/](./resource) contains various config files, shaders and tilesheets.
- [lib/](./lib) contains the third-party dependencies for this project.

## Acknowledgements
Thanks to Joey de Vries for their tutorial on
OpenGL. [[Tutorial]](https://learnopengl.com/) [[GitHub]](https://github.com/JoeyDeVries/LearnOpenGL)