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

#include <vector>

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/GridLines.hpp>

namespace EconSimPlusPlus {
    GridLines::GridLines(const glm::ivec2 size, const glm::vec2 cellSize) {
        std::vector<float> vertices{};
        const auto halfSize{static_cast<glm::vec2>(size) * cellSize * 0.5f};

        // Horizontal Lines
        for (int row = 0; row <= size.y; ++row) {
            const float y{static_cast<float>(row) * cellSize.y - halfSize.y};
            // Start Point
            vertices.push_back(-halfSize.x);
            vertices.push_back(y);
            // End Point
            vertices.push_back(halfSize.x);
            vertices.push_back(y);
        }

        // Vertical Lines
        for (int col = 0; col <= size.x; ++col) {
            const float x{static_cast<float>(col) * cellSize.x - halfSize.x};
            // Start Point
            vertices.push_back(x);
            vertices.push_back(-halfSize.y);
            // End Point
            vertices.push_back(x);
            vertices.push_back(halfSize.y);
        }

        vao.bind();
        vbo.bind();
        vbo.loadData(vertices, {2});

        setSize({static_cast<glm::vec2>(size), 1.0f});
    }

    void GridLines::render(const Camera& camera, const float z) const {
        shader.bind();
        shader.setUniform("color", glm::vec3{1.0f});
        shader.setUniform("projectionViewMatrix", camera.getPerspectiveMatrix() * camera.getViewMatrix());
        shader.setUniform("transform", glm::translate(m_transform, glm::vec3{0.0f, 0.0f, z}));
        vao.bind();
        vbo.drawArrays(GL_LINES);
    }
} // namespace EconSimPlusPlus
