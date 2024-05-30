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
// Created by Anthony Dickson on 30/05/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_QUAD_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_QUAD_HPP

#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {
    /// A generic quad of unit size.
    /// @note The origin is at the bottom left corner. You should use a shader to transform (e.g., scale, shift) the
    /// vertices.
    class Quad {
    public:
        Quad();
        Quad(Quad&) = delete; // Prevent issues with OpenGL stuff.

        /// Draw the quad.
        /// @param mode How to draw the vertex data.
        void render(GLenum mode = GL_TRIANGLE_STRIP) const;

        /// Draw the quad with instancing.
        /// @param instanceCount The number of instances to render.
        /// @param mode How to draw the vertex data.
        void render(int instanceCount, GLenum mode = GL_TRIANGLE_STRIP) const;

    private:
        VertexArray m_vao{};
        VertexBuffer m_vbo{};
    };
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_QUAD_HPP
