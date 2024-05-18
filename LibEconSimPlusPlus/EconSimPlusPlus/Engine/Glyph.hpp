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
// Created by Anthony Dickson on 26/04/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_GLYPH_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_GLYPH_HPP

#include "glm/vec2.hpp"

namespace EconSimPlusPlus::Engine {
    /// Represents a single character in a TrueType font.
    struct Glyph {
        /// The character this glyph represents.
        const unsigned char character;
        /// The width and height of the character.
        const glm::vec2 size;
        /// The horizontal and vertical offset to sit letters on the baseline.
        const glm::vec2 bearing;
        /// The spacing between this character and other characters.
        const float advance;

        /// Create a new Glyph.
        /// @param character_ The character this glyph represents.
        /// @param size_ The width and height of the character.
        /// @param bearing_ The horizontal and vertical offset to sit letters on the baseline.
        /// @param advance_ The spacing between this character and other characters.
        Glyph(unsigned char character_, glm::vec2 size_, glm::vec2 bearing_, float advance_);

        Glyph(Glyph&) = delete;
        Glyph(Glyph&&) = delete;
    };
} // namespace EconSimPlusPlus::Engine


#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_GLYPH_HPP
