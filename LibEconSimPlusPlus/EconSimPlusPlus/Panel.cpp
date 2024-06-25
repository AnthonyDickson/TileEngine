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
    Panel::Panel(const glm::vec2 position, const PanelSettings& settings) : m_settings(settings) {
        Object::setPosition(position);
        Object::setAnchor(m_settings.anchor);

        m_nextObjectPosition = topLeft(*this);
    }

    void Panel::addObject(std::unique_ptr<Object> object) {
        if (object->size().x > size().x) {
            setSize(glm::vec2{object->size().x, size().y});
        }

        if (m_nextObjectPosition.y - object->size().y < bottomLeft(*this).y) {
            setSize(glm::vec2{size().x, size().y + object->size().y});
        }

        object->setAnchor(Anchor::topLeft);
        object->setPosition(m_nextObjectPosition);
        object->setLayer(layer());

        m_nextObjectPosition = bottomLeft(*object);
        m_objects.push_back(std::move(object));
    }

    void Panel::setPosition(const glm::vec2 position) {
        Object::setPosition(position);
        recalculateObjectPositions();
    }

    void Panel::setSize(const glm::vec2 size) {
        Object::setSize(size);
        recalculateObjectPositions();
    }

    void Panel::setLayer(const float layer) {
        Object::setLayer(layer);

        for (const auto& object : m_objects) {
            object->setLayer(layer);
        }
    }

    void Panel::notify(const Event event, const EventData eventData) {
        Object::notify(event, eventData);

        for (const auto& object : m_objects) {
            object->notify(event, eventData);
        }
    }

    void Panel::update(const float deltaTime, const InputState& inputState, const Camera& camera) {
        for (const auto& object : m_objects) {
            object->update(deltaTime, inputState, camera);
        }
    }

    void Panel::render(const Camera& camera) const {
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
        m_shader.setUniform("color", m_settings.fillColor);
        m_quad.render();

        drawOutline(*this, m_shader, m_quad, m_settings.outlineColor, m_settings.outlineThickness);

        for (const auto& object : m_objects) {
            object->render(camera);
        }
    }

    void Panel::recalculateObjectPositions() {
        m_nextObjectPosition = topLeft(*this);

        for (auto& object : m_objects) {
            object->setPosition(m_nextObjectPosition);
            m_nextObjectPosition = bottomLeft(*object);
        }
    }
} // namespace EconSimPlusPlus
