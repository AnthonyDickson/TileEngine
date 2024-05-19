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

#ifndef LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_GUIOBJECT_HPP
#define LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_GUIOBJECT_HPP

#include "glm/ext/matrix_common.hpp"
#include "glm/vec2.hpp"

#include <EconSimPlusPlus/Engine/Camera.hpp>
#include <EconSimPlusPlus/Engine/InputState.hpp>

namespace EconSimPlusPlus::Editor {

    class GUIObject {
    public:
        virtual ~GUIObject();

        /// Get the object's 2D coordinate in world space.
        /// @return A 2-vector.
        [[nodiscard]] glm::vec2 position() const;

        /// Set the 2D coordinates of an object.
        /// @param position The object's 2D coordinate in world space.
        void setPosition(glm::vec2 position);

        /// The layer the object exists on.
        /// This controls the ordering during rendering. Higher values put the object closer to the camera.
        /// @return A non-negative number.
        [[nodiscard]] float layer() const;

        /// Set the object's layer.
        /// @param layer The layer the object exists on. Higher values put the object closer to the camera.
        void setLayer(float layer);

        /// Get the object's dimensions.
        /// @return A 2-vector.
        [[nodiscard]] glm::vec2 size() const;

        /// Set the size of the object.
        /// @param size The 2D dimensions of the object in pixels.
        void setSize(glm::vec2 size);

        /// Update the object.
        /// @param deltaTime The size of the step to take in terms of time (seconds).
        /// @param inputState The state of keyboard and mouse input.
        /// @param camera
        virtual void update(float deltaTime, const Engine::InputState& inputState, const Engine::Camera& camera) = 0;

        /// Draw the object on screen.
        /// @param camera The camera to render the tile map with.
        virtual void render(const Engine::Camera& camera) const = 0;

        /// Check whether a point is contained in the object's axis-aligned bounding box.
        /// @param point The 2D point to test.
        /// @return Whether the point interects the object.
        [[nodiscard]] bool contains(glm::vec2 point) const;

    private:
        /// The screen space coordinates of the object along with the layer. Note this refers to the top left of the
        /// object.
        glm::vec3 m_position{1.0f};
        /// The width and height of the object in world space coordinates.
        glm::vec2 m_size{1.0f};
    };

} // namespace EconSimPlusPlus::Editor

#endif // LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_GUIOBJECT_HPP
