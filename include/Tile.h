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
// Created by Anthony on 7/04/2024.
//

#ifndef ECONSIMPLUSPLUS_TILE_H
#define ECONSIMPLUSPLUS_TILE_H


#include <memory>
#include <utility>
#include "glm/vec2.hpp"
#include "Texture.h"

class Tile {
private:
    /** ID is used in tile arrays for compact storage of maps (levels) and for placing tiles in maps/sets. */
    int id;
    /** The texture to bind to draw this tile. */
    std::shared_ptr<const Texture> texture;
public:
    Tile(int id_, std::shared_ptr<const Texture> texture_);

    /** Bind OpenGL related stuff for rendering. */
    void bind() const;
};


#endif //ECONSIMPLUSPLUS_TILE_H
