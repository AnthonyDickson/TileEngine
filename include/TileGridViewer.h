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

#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "KeyboardState.h"
#include "Size.h"
#include "TileGrid.h"

struct MatrixTileIDPair {
    glm::mat4 transform;
    int tileID;
};

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
    const Size<int> viewport;

public:
    /**
     * Create a view of a tile grid.
     * @param tileGrid_ The tile grid to provide a view of.
     * @param viewport_ The how many tiles to show.
     */
    explicit TileGridView(std::shared_ptr<const TileGrid> tileGrid_, const Size<int>& viewport_);

    /**
     * Respond to user input.
     * @param keyboardState The state of the keyboard keys.
     */
    void processInput(const KeyboardState& keyboardState);

    /**
     * Get the transform and IDs of tiles to draw on screen.
     * @param tileSize The size (width and height) to draw tiles at (in pixels).
     * @return A list of tile transform and ID pairs to draw.
     */
    [[nodiscard]] std::vector<MatrixTileIDPair> getTilePositionAndIds(float tileSize) const;
};



#endif //TILEGRIDVIEWER_H
