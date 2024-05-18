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
// Created by Anthony Dickson on 04/05/2024.
//

#ifndef TEXTUREARRAY_HPP
#define TEXTUREARRAY_HPP

#include <memory>

#include <glm/vec2.hpp>

namespace EconSimPlusPlus::Engine {

    /// A collection of textures stored in an OpenGL texture array.
    /// @note Assumes single channel textures.
    class TextureArray {
    public:
        /// Create an empty texture array with the given depth and resolution.
        /// @param depth_ The depth of the texture array, i.e., how many sub textures the array holds.
        /// @param resolution_ The width and height of each sub texture in pixels.
        /// @return An empty texture array.
        static std::unique_ptr<TextureArray> create(int depth_, glm::ivec2 resolution_);

        /// Create a texture array from an OpenGL ID.
        /// @param id_ The OpenGL ID for the texture array.
        explicit TextureArray(unsigned int id_);

        TextureArray(TextureArray&) = delete; // Prevent copy to avoid issues with textures being freed via destructor.

        ~TextureArray();

        /// Load a texture into the texture array.
        /// @param zOffset The "depth" or "index" of the sub texture.
        /// @param bufferSize The width and height of the buffer in pixels.
        /// @param buffer The raw image buffer (single channel).
        void bufferSubImage(int zOffset, glm::ivec2 bufferSize, const unsigned char* buffer) const;

        /// Bind the texture array for rendering.
        void bind() const;

    private:
        /// The OpenGL ID for the texture array.
        const unsigned int m_id{};
    };

} // namespace EconSimPlusPlus::Engine

#endif // TEXTUREARRAY_HPP
