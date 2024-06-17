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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_OBJECT_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_OBJECT_HPP

#include <functional>

#include "glm/ext/matrix_common.hpp"
#include "glm/vec2.hpp"

#include <EconSimPlusPlus/Anchor.hpp>
#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/Event.hpp>
#include <EconSimPlusPlus/InputState.hpp>

namespace EconSimPlusPlus {

    class Object {
    public:
        virtual ~Object();

        /// Get the object's 2D coordinate in world space.
        /// @return A 2-vector.
        [[nodiscard]] glm::vec2 position() const;

        /// Set the 2D coordinates of an object.
        /// @param position The object's 2D coordinate in world space.
        virtual void setPosition(glm::vec2 position);

        /// The layer the object exists on.
        /// This controls the ordering during rendering. Higher values put the object closer to the camera.
        /// @return A non-negative number.
        [[nodiscard]] float layer() const;

        /// Set the object's layer.
        /// @param layer The layer the object exists on. Higher values put the object closer to the camera.
        virtual void setLayer(float layer);

        /// Get the scale of the object.
        /// @return The non-negative scale factor of the object along the x and y axis.
        [[nodiscard]] glm::vec2 scale() const;

        /// Set the scale of the object.
        /// @param scale The non-negative scale factor of the object along the x and y axis.
        void setScale(glm::vec2 scale);

        /// Get the object's dimensions.
        /// @return A 2-vector.
        [[nodiscard]] glm::vec2 size() const;

        /// Set the size of the object.
        /// @param size The 2D dimensions of the object in pixels.
        virtual void setSize(glm::vec2 size);

        /// Get the 4x4 homogeneous transform matrix of the object.
        /// @note Assumes bottom left as position/origin.
        [[nodiscard]] glm::mat4 transform() const;

        /// Get the point on the object that is used as the origin for its position.
        /// @return An anchor.
        [[nodiscard]] Anchor anchor() const;

        /// Set the object's anchor point.
        /// @param anchor The point on the object that is used as the origin for its position.
        virtual void setAnchor(Anchor anchor);

        /// Register an event handler for this object.
        /// @param eventHandler A function that responds to events.
        void addEventHandler(const std::function<void(Event event)>& eventHandler);

        /// Tell the object that an event has occured.
        /// @param event The event that has occured.
        void notify(Event event);

        /// Update the object.
        /// @param deltaTime The size of the step to take in terms of time (seconds).
        /// @param inputState The state of keyboard and mouse input.
        /// @param camera The main camera. Can be used to convert between screen space and world space coordinates.
        virtual void update(float deltaTime, const InputState& inputState, const Camera& camera) = 0;

        /// Draw the object on screen.
        /// @param camera The camera to render the tile map with.
        virtual void render(const Camera& camera) const = 0;

        /// Check whether a point is contained in the object's axis-aligned bounding box.
        /// @param point The 2D point to test.
        /// @return Whether the point interects the object.
        [[nodiscard]] bool contains(glm::vec2 point) const;

    private:
        /// The world space coordinates of the object along with the layer. Note this refers to the top left of the
        /// object.
        glm::vec3 m_position{0.0f};
        /// The non-negative scale factor of the object along the x and y axis.
        glm::vec2 m_scale{1.0f};
        /// The width and height of the object in world space coordinates.
        glm::vec2 m_size{1.0f};
        /// The point on the object that is used as the origin for its position.
        Anchor m_anchor{Anchor::topLeft};
        /// The registered event handlers.
        std::vector<std::function<void(Event event)>> m_eventHandlers{};
    };

    /// Get the coordinates of the top left corner of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] glm::vec2 topLeft(const Object& object);

    /// Get the coordinates of the bottom left corner of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] glm::vec2 bottomLeft(const Object& object);

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_OBJECT_HPP
