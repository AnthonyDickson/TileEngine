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

#include "TileGrid.h"

#include "KeyboardState.h"

TileGrid::TileGrid(const int width_, const int height_) : grid(width_ * height_, 0), width(width_), height(height_) {}

int &TileGrid::operator[](const std::size_t row, const std::size_t col) {
    return grid[width * row + col];
}

const int& TileGrid::get(const std::size_t row, const std::size_t col) const {
    return grid[width * row + col];
}

int& TileGrid::at(const std::size_t row, const std::size_t col) {
    return grid[width * row + col];
}
