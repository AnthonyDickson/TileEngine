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
// Created by Anthony on 8/04/2024.
//

#ifndef ECONSIMPLUSPLUS_TILEGRID_H
#define ECONSIMPLUSPLUS_TILEGRID_H

#include <vector>

/** Represents a 2D grid of tile IDs. */
class TileGrid {
private:
    /** A flat array storing the tile IDS. */
    std::vector<int> grid;

public:
    /** How wide the grid is in cells along the x-axis. */
    const int width;
    /** How tall the grid is in cells along the y-axis. */
    const int height;

    /**
     * Create a TileGrid.
     * @param width_ How wide the grid is in cells along the x-axis.
     * @param height_ How tall the grid is in cells along the y-axis.
     */
    TileGrid(int width_, int height_);

    /**
     * Access tile cells.
     * @param row The grid coordinates along the y-axis.
     * @param col The grid coordinates along the x-axis.
     * @return The tile ID at the given coordinates.
     */
    int& operator[](std::size_t row, std::size_t col);

    /**
     * Read-only access to tile cells.
     * @param row The grid coordinates along the y-axis.
     * @param col The grid coordinates along the x-axis.
     * @return The tile ID at the given coordinates.
     */
    [[nodiscard]] const int& get(std::size_t row, std::size_t col) const;

    /**
     * Read and write access to tile cells.
     * @param row The grid coordinates along the y-axis.
     * @param col The grid coordinates along the x-axis.
     * @return The tile ID at the given coordinates.
     */
    [[nodiscard]] int& at(std::size_t row, std::size_t col);
};


#endif //ECONSIMPLUSPLUS_TILEGRID_H
