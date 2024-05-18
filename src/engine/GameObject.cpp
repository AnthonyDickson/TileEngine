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
// Created by Anthony Dickson on 12/05/2024.
//

#include <EconSimPlusPlus/Engine/GameObject.hpp>

namespace EconSimPlusPlus::Engine {
    GameObject::~GameObject() = default;

    glm::mat4 GameObject::transform() const {
        return m_transform;
    }

    void GameObject::setTransform(const glm::mat4& transform) {
        m_transform = transform;
    }

    glm::vec2 GameObject::position() const {
        return {m_transform[3][0], m_transform[3][1]};
    }

    void GameObject::setPosition(const glm::vec2 position) {
        m_transform[3][0] = position.x;
        m_transform[3][1] = position.y;
    }

    float GameObject::layer() const {
        return m_transform[3][2];
    }

    void GameObject::setLayer(const float layer) {
        assert(layer >= 0.0f && "Layer must be non-negative.");
        m_transform[3][2] = layer;
    }

    glm::vec2 GameObject::size() const {
        return m_size;
    }

    void GameObject::setSize(const glm::vec2 size) {
        assert(glm::all(glm::greaterThanEqual(size, glm::vec2{1.0f})) &&
               "All components of size must be greater than or equal to 1.0.");
        m_size = size;
    }

    bool GameObject::contains(glm::vec2 point) const {
        for (int axis = 0; axis < 2; ++axis) {
            constexpr int translationCol = 3;
            const bool exceedsMinExtent = point[axis] < m_transform[translationCol][axis];
            // ReSharper disable once CppTooWideScopeInitStatement
            const bool exceedsMaxExtent =
                point[axis] > m_transform[translationCol][axis] + m_size[axis];

            if (exceedsMinExtent or exceedsMaxExtent) {
                return false;
            }
        }

        return true;
    }
} // namespace EconSimPlusPlus::Engine
