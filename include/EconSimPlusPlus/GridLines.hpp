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
// Created by Anthony Dickson on 10/05/2024.
//

#ifndef GRIDLINES_HPP
#define GRIDLINES_HPP

#include "glm/vec2.hpp"

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/GameObject.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {
    /// Draws 2D grid lines.
    class GridLines final : public GameObject {
    public:
        /// Create a grid lines object.
        /// @param size The width and height of the grid in tiles.
        /// @param cellSize The width and height of the cells in pixels.
        GridLines(glm::ivec2 size, glm::vec2 cellSize);

        GridLines(GridLines&) = delete; // Prevent copy to avoid issues w/ OpenGL

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;

        /// Draw the grid lines on screen.
        /// @param camera The camera to render the grid lines with.
        void render(const Camera& camera) const override;

    private:
        /// The gap between lines.
        const glm::vec2 m_cellSize;
        /// The vertex array object.
        const VertexArray m_vao{};
        /// The vertex buffer object.
        VertexBuffer m_vbo{};
        /// The shader for drawing grid lines.
        const Shader m_shader{Shader::create("resource/shader/grid.vert", "resource/shader/grid.frag")};
    };

} // namespace EconSimPlusPlus

#endif // GRIDLINES_HPP
