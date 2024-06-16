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
// Created by Anthony Dickson on 16/06/2024.
//

#include "Object.hpp"

#include "glm/ext/matrix_transform.hpp"

namespace EconSimPlusPlus {
    Object::~Object() = default;

    glm::vec2 Object::position() const {
        return {m_position.x, m_position.y};
    }

    void Object::setPosition(const glm::vec2 position) {
        m_position.x = position.x;
        m_position.y = position.y;
    }

    float Object::layer() const {
        return m_position.z;
    }

    void Object::setLayer(const float layer) {
        assert(layer >= 0.0f && "Layer must be non-negative.");
        m_position.z = layer;
    }

    glm::vec2 Object::scale() const {
        return m_scale;
    }

    void Object::setScale(const glm::vec2 scale) {
        assert(glm::all(glm::greaterThanEqual(scale, glm::vec2{0.0f})) &&
               "All components of scale must be greater than or equal to 0.0.");
        m_scale = scale;
    }

    glm::vec2 Object::size() const {
        return m_size;
    }

    void Object::setSize(const glm::vec2 size) {
        assert(glm::all(glm::greaterThanEqual(size, glm::vec2{0.0f})) &&
               "All components of size must be greater than or equal to 0.0.");
        m_size = size;
    }

    glm::mat4 Object::transform() const {
        return glm::scale(glm::translate(glm::mat4{1.0f}, glm::vec3{bottomLeft(*this), layer()}),
                          glm::vec3{scale(), 1.0f});
    }

    Anchor Object::anchor() const {
        return m_anchor;
    }

    void Object::setAnchor(const Anchor anchor) {
        m_anchor = anchor;
    }

    void Object::addEventHandler(const std::function<void(Event event)>& eventHandler) {
        m_eventHandlers.push_back(eventHandler);
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void Object::notify(const Event event) {
        for (auto& handler : m_eventHandlers) {
            handler(event);
        }
    }

    bool Object::contains(glm::vec2 point) const {
        // Using `m_size.y` as the vertical baseline makes the top left corner the origin.
        const glm::vec2 anchorOffset{calculateAnchorOffset(m_size, m_anchor, m_size.y)};

        for (int axis = 0; axis < 2; ++axis) {
            const bool exceedsMinExtent = point[axis] < anchorOffset[axis] + m_position[axis];
            // ReSharper disable once CppTooWideScopeInitStatement
            const bool exceedsMaxExtent = point[axis] > anchorOffset[axis] + m_position[axis] + m_size[axis];

            if (exceedsMinExtent or exceedsMaxExtent) {
                return false;
            }
        }

        return true;
    }

    glm::vec2 topLeft(const Object& object) {
        const glm::vec2 anchorOffset{calculateAnchorOffset(object.size(), object.anchor())};

        return object.position() + anchorOffset;
    }

    glm::vec2 bottomLeft(const Object& object) {
        const glm::vec2 topLeft{EconSimPlusPlus::topLeft(object)};

        return topLeft - glm::vec2{0.0f, object.size().y};
    }
} // namespace EconSimPlusPlus
