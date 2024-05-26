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
// Created by Anthony Dickson on 19/05/2024.
//

#include <EconSimPlusPlus/Engine/GuiObject.hpp>


namespace EconSimPlusPlus::Engine {
    GUIObject::~GUIObject() = default;

    glm::vec2 GUIObject::position() const {
        return {m_position.x, m_position.y};
    }

    void GUIObject::setPosition(const glm::vec2 position) {
        m_position.x = position.x;
        m_position.y = position.y;
    }

    float GUIObject::layer() const {
        return m_position.z;
    }

    void GUIObject::setLayer(const float layer) {
        assert(layer >= 0.0f && "Layer must be non-negative.");
        m_position.z = layer;
    }

    glm::vec2 GUIObject::size() const {
        return m_size;
    }

    void GUIObject::setSize(const glm::vec2 size) {
        assert(glm::all(glm::greaterThanEqual(size, glm::vec2{0.0f})) &&
               "All components of size must be greater than or equal to 0.0.");
        m_size = size;
    }

    Anchor GUIObject::anchor() const {
        return m_anchor;
    }

    void GUIObject::setAnchor(const Anchor anchor) {
        m_anchor = anchor;
    }

    bool GUIObject::contains(glm::vec2 point) const {
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
} // namespace EconSimPlusPlus::Engine
