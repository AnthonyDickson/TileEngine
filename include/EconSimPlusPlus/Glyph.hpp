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

#ifndef GLYPH_HPP
#define GLYPH_HPP

#include "glad/glad.h"
#include "glm/vec2.hpp"

namespace EconSimPlusPlus {
    /// Represents a single character in a TrueType font.
    class Glyph {
    private:
        // TODO: Use Texture object
        /// The OpenGL ID of the character bitmap.
        const GLuint textureID;

    public:
        /// The width and height of the character.
        const glm::ivec2 size;
        /// The horizontal and vertical offset to sit letters on the baseline.
        const glm::ivec2 bearing;
        /// The spacing between this character and other characters.
        const long advance;

        /// Create a new Glyph.
        /// @param textureID_ The OpenGL ID of the character bitmap.
        /// @param size_ The width and height of the character.
        /// @param bearing_ The horizontal and vertical offset to sit letters on the baseline.
        /// @param advance_ The spacing between this character and other characters.
        Glyph(GLuint textureID_, glm::ivec2 size_, glm::ivec2 bearing_, long advance_);

        Glyph(Glyph&) = delete;
        Glyph(Glyph&&) = delete;

        /// Clean up OpenGL stuff.
        ~Glyph();

        /// Bind the glyph texture for rendering.
        void bind() const;
    };
} // namespace EconSimPlusPlus


#endif // GLYPH_HPP
