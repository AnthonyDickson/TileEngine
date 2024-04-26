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
// Created by Anthony Dickson on 25/04/2024.
//

#ifndef TILETYPE_H
#define TILETYPE_H

#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {
    /** Hold the properties of a tile type. */
    struct TileType {
        std::unique_ptr<VertexArray> vao;
        std::unique_ptr<VertexBuffer> vbo;

        explicit TileType(std::unique_ptr<VertexArray> vao_, std::unique_ptr<VertexBuffer> vbo_) :
            vao(std::move(vao_)), vbo(std::move(vbo_)) {
        }
    };
} // namespace EconSimPlusPlus

#endif // TILETYPE_H
