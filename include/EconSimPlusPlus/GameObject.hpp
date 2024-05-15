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

#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "glm/matrix.hpp"

namespace EconSimPlusPlus {

    /// The general base object that represents physical objects that can be placed in a game world.
    class GameObject {
    public:
        virtual ~GameObject();

        /// Get the object's transform (position, orientation, scale).
        /// @return A 4x4 homogenous transform matrix.
        [[nodiscard]] glm::mat4 transform() const;

        /// Set the transform matrix.
        /// @param transform The new homogeneous transform matrix.
        void setTransform(const glm::mat4& transform);

        /// Get the object's dimensions.
        /// @return A 3-vector.
        [[nodiscard]] glm::vec3 size() const;

        /// Set the size of the object.
        /// @param size The 3D dimensions of the object in pixels.
        void setSize(glm::vec3 size);

        /// Check whether a point is contained in the object's axis-aligned bounding box.
        /// @param point The 2D point to test.
        /// @return Whether the point interects the object.
        [[nodiscard]] bool contains(glm::vec2 point) const;

    protected:
        /// The 4x4 homogeneous transform matrix representing the object's position, orientation and scale.
        /// Corresponds to the model matrix in the model-view-projection matrix triplet.
        glm::mat4 m_transform{1.0f};
        /// The width, height and depth of the object in world space coordinates.
        glm::vec3 m_size{1.0f};
    };

} // namespace EconSimPlusPlus

#endif // GAMEOBJECT_HPP
