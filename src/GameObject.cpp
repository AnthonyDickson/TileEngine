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

#include <EconSimPlusPlus/GameObject.hpp>

namespace EconSimPlusPlus {
    GameObject::~GameObject() = default;

    glm::mat4 GameObject::transform() const {
        return m_transform;
    }

    void GameObject::setTransform(const glm::mat4& transform) {
        m_transform = transform;
    }

    glm::vec3 GameObject::size() const {
        return m_size;
    }

    void GameObject::setSize(const glm::vec3 size) {
        assert(glm::greaterThanEqual(size, 1.0f) && "All components of size must be greater than or equal to 1.0.");
        m_size = size;
    }

    bool GameObject::contains(glm::vec3 point) const {
        for (int axis = 0; axis < 3; ++axis) {
            constexpr int translationCol = 3;
            const bool exceedsMinExtent = point[axis] < m_transform[translationCol][axis];
            // ReSharper disable once CppTooWideScopeInitStatement
            const bool exceedsMaxExtent =
                point[axis] > m_transform[translationCol][axis] + m_transform[axis][axis] * m_size[axis];

            if (exceedsMinExtent or exceedsMaxExtent) {
                return false;
            }
        }

        return true;
    }
} // namespace EconSimPlusPlus
