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
// Created by Anthony Dickson on 05/06/2024.
//

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Outline.hpp>


namespace EconSimPlusPlus {
    void drawOutline(const Object& object, const Shader& shader, const Quad& quad, const glm::vec3 color,
                     const float thickness, const OutlinePlacement placement) {
        if (thickness < 1.0f) {
            return;
        }

        glm::vec2 origin{bottomLeft(object)};

        const auto renderBorder = [&](const glm::vec2& bottomLeftCorner, const glm::vec2& topRightCorner) {
            glm::mat4 transform = glm::translate(glm::mat4{1.0f}, {bottomLeftCorner, object.layer()});
            transform = glm::scale(transform, {topRightCorner - bottomLeftCorner, 1.0f});
            shader.setUniform("transform", transform);
            shader.setUniform("color", color);
            quad.render();
        };

        glm::vec2 bottomLeft;
        glm::vec2 topRight;

        switch (placement) {
        case OutlinePlacement::inset:
            // Left side
            bottomLeft = origin;
            topRight = {origin.x + thickness, origin.y + object.size().y};
            renderBorder(bottomLeft, topRight);
            // Right side
            bottomLeft = {origin.x + object.size().x - thickness, origin.y};
            topRight = {origin.x + object.size().x, origin.y + object.size().y};
            renderBorder(bottomLeft, topRight);
            // Bottom side
            bottomLeft = origin;
            topRight = {origin.x + object.size().x, origin.y + thickness};
            renderBorder(bottomLeft, topRight);
            // Top side
            bottomLeft = {origin.x, origin.y + object.size().y - thickness};
            topRight = {origin.x + object.size().x, origin.y + object.size().y};
            renderBorder(bottomLeft, topRight);
            break;
        case OutlinePlacement::outset:
            // Left side
            bottomLeft = {origin.x - thickness, origin.y - thickness};
            topRight = {origin.x, bottomLeft.y + object.size().y + thickness};
            renderBorder(bottomLeft, topRight);
            // Right side
            bottomLeft = {origin.x + object.size().x, origin.y - thickness};
            topRight = {origin.x + object.size().x + thickness, bottomLeft.y + object.size().y + thickness};
            renderBorder(bottomLeft, topRight);
            // Bottom side
            bottomLeft = {origin.x - thickness, origin.y - thickness};
            topRight = {origin.x + object.size().x + thickness, origin.y};
            renderBorder(bottomLeft, topRight);
            // Top side
            bottomLeft = {origin.x - thickness, origin.y + object.size().y};
            topRight = {origin.x + object.size().x + thickness, origin.y + object.size().y + thickness};
            renderBorder(bottomLeft, topRight);
            break;
        }
    }
} // namespace EconSimPlusPlus
