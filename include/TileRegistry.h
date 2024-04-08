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

#ifndef ECONSIMPLUSPLUS_TILEREGISTRY_H
#define ECONSIMPLUSPLUS_TILEREGISTRY_H


#include <vector>
#include <memory>

#include "Texture.h"
#include "Tile.h"

/** Registers Tile objects to an integer ID. */
class TileRegistry {
private:
    /** The registered tiles. */
    std::vector<Tile> tiles{};

public:
    /**
     * Instantiate and add a new Tile object with the given texture.
     * @param texture The texture to use for the tile.
     */
    void emplace(std::shared_ptr<const Texture> &texture);

    /**
     * Access a tile by ID.
     * @param tileID The ID of the Tile to retrieve.
     * @return A Tile object.
     */
    const Tile &operator[](int tileID);
};


#endif //ECONSIMPLUSPLUS_TILEREGISTRY_H
