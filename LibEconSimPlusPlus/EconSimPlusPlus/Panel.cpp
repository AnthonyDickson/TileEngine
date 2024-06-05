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
// Created by Anthony Dickson on 04/06/2024.
//

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Outline.hpp>
#include <EconSimPlusPlus/Panel.hpp>

namespace EconSimPlusPlus {
    Panel::Panel(const glm::vec2 position, const glm::vec2 size, const Anchor anchor) {
        GUIObject::setPosition(position);
        setSize(size);
        GUIObject::setAnchor(anchor);
    }

    void Panel::update(float, const InputState&, const Camera&) {
    }

    void Panel::render(const Camera& camera) const {
        // TODO: Wrap panel settings in struct.
        constexpr glm::vec3 fillColor{0.3f};
        constexpr float outlineThickness{1.0f};
        constexpr glm::vec3 outlineColor{0.6f};

        m_shader.bind();

        // Need to add this to camera projection-view matrix otherwise z sorting order will not match other objects.
        const glm::mat4 cameraViewZ = glm::translate(glm::mat4{1.0f}, {0.0f, 0.0f, -camera.position().z});
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * cameraViewZ);

        // Draw the button fill color.
        const glm::vec2 anchorOffset{calculateAnchorOffset(size(), anchor(), size().y)};
        glm::vec2 offsetPosition{position() + anchorOffset};
        glm::mat4 transform{glm::translate(glm::mat4{1.0f}, {offsetPosition, layer()})};
        transform = glm::scale(transform, {size(), 1.0f});
        m_shader.setUniform("transform", transform);
        m_shader.setUniform("color", fillColor);
        m_quad.render(GL_TRIANGLE_STRIP);

        drawOutline(*this, m_shader, m_quad, outlineColor, outlineThickness);
    }
} // namespace EconSimPlusPlus
