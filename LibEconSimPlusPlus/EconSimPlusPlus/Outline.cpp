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
    void drawOutline(const Object& object, const Shader& shader, const Quad& quad, const glm::vec3 outlineColor,
                     const float outlineThickness) {
        if (outlineThickness < 1.0f) {
            return;
        }

        const glm::vec2 anchorOffset{calculateAnchorOffset(object.size(), object.anchor(), object.size().y)};
        glm::vec2 offsetPosition{object.position() + anchorOffset};

        const auto renderBorder = [&](const glm::vec2& bottomLeftCorner, const glm::vec2& topRightCorner) {
            glm::mat4 transform = glm::translate(glm::mat4{1.0f}, {bottomLeftCorner, object.layer()});
            transform = glm::scale(transform, {topRightCorner - bottomLeftCorner, 1.0f});
            shader.setUniform("transform", transform);
            shader.setUniform("color", outlineColor);
            quad.render();
        };

        // Left side
        glm::vec2 bottomLeft{offsetPosition};
        glm::vec2 topRight{bottomLeft.x + outlineThickness, bottomLeft.y + object.size().y};
        renderBorder(bottomLeft, topRight);
        // Right side
        bottomLeft = {offsetPosition.x + object.size().x - outlineThickness, offsetPosition.y};
        topRight = {bottomLeft.x + outlineThickness, bottomLeft.y + object.size().y};
        renderBorder(bottomLeft, topRight);
        // Bottom side
        bottomLeft = offsetPosition;
        topRight = {bottomLeft.x + object.size().x, bottomLeft.y + outlineThickness};
        renderBorder(bottomLeft, topRight);
        // Top side
        bottomLeft = {offsetPosition.x, offsetPosition.y + object.size().y - outlineThickness};
        topRight = {bottomLeft.x + object.size().x, bottomLeft.y + outlineThickness};
        renderBorder(bottomLeft, topRight);
    }
} // namespace EconSimPlusPlus
