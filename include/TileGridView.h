//  EconSimPlusPlus
//  A program that simulates the historical development of capitalist economies.
//  Copyright (C) 2024.   Anthony Dickson anthony.dickson9656@gmail.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by Anthony Dickson on 14/04/2024.
//

#ifndef TILEGRIDVIEWER_H
#define TILEGRIDVIEWER_H

#include "KeyboardInput.h"
#include "Shader.h"
#include "Size.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "TileGrid.h"
#include "TileRegistry.h"

class TileGridView {
    /** Handles a 'camera view' of a tile grid. */
private:
    /** The camera position (left edge of view) along the x-axis. */
    int rowOffset{};
    /** The camera position (top edge of view) along the y-axis. */
    int colOffset{};

    /** A pointer to the tile grid this object will view. */
    std::shared_ptr<const TileGrid> tileGrid;
    /** The size of the viewport in tiles. */
    Size<int> viewport;
    /** The size of each tile in pixels. */
    int tileSize;

    /** Holds the vertex data for rendering a tile. */
    const VertexArray vao{};
    /** The buffer for the vertex data. */
    VertexBuffer vbo{};
    /** The shader for rendering tiles. */
    const Shader shader{"resource/shader/tile.vert", "resource/shader/tile.frag"};

public:
    /**
     * Create a view of a tile grid.
     * @param tileGrid_ The tile grid to provide a view of.
     * @param viewport_ The how many tiles to show.
     * @param tileSize_ The size of the tiles (height and width) in pixels.
     */
    TileGridView(std::shared_ptr<const TileGrid> tileGrid_, const Size<int>& viewport_, int tileSize_);

    /** Delete copy constructor to avoid OpenGL issues. */
    TileGridView(TileGridView&) = delete;
    /** Delete move constructor to avoid OpenGL issues. */
    TileGridView(TileGridView&&) = delete;

    /**
     * Update the viewport size.
     * @param windowSize The new size of the window in pixels.
     */
    void updateViewport(Size<int> windowSize);

    /**
     * Respond to user input.
     * @param keyboardState The state of the keyboard keys.
     */
    void processInput(const KeyboardInput& keyboardState);

    /**
     * Draw the view of the tile grid on the screen.
     * @param projectionViewMatrix The perspective matrix multiplied by the view matrix.
     */
    void render(const glm::mat4& projectionViewMatrix) const;
private:
    /**
     * Setter for `rowOffset` that ensures that `rowOffset` is always a valid value.
     * @param value The value to set `rowOffset` to.
     */
    void setRowOffset(int value);

    /**
     * Setter for `colOffset` that ensures that `colOffset` is always a valid value.
     * @param value The value to set `colOffset` to.
     */
    void setColOffset(int value);
};



#endif //TILEGRIDVIEWER_H
