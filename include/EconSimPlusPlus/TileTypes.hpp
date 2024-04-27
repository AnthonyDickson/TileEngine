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

#ifndef TILETYPES_H
#define TILETYPES_H

#include <EconSimPlusPlus/Size.hpp>
#include <EconSimPlusPlus/TileType.hpp>

namespace EconSimPlusPlus {
    /// A collection of tile types.
    class TileTypes {
    private:
        /// The list of tile types.
        std::vector<TileType> tileTypes{};

    public:
        /// Generate the set of tile type info for a given tile sheet size.
        /// @param sheetSize The width and height (in tiles) of the tile sheet.
        /// @return A collection of tile types.
        static std::unique_ptr<TileTypes> create(Size<int> sheetSize);

        /// Render the tile geometry.
        /// *Note:* Requires the textures to be bound outside this function.
        void render(int tileID) const;
    };
} // namespace EconSimPlusPlus

#endif // TILETYPES_H
