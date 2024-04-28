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
// Created by Anthony on 26/03/2024.
//

#ifndef ECONSIMPLUSPLUS_CAMERA_H
#define ECONSIMPLUSPLUS_CAMERA_H

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/vec3.hpp"

#include <EconSimPlusPlus/Direction.hpp>
#include <EconSimPlusPlus/InputState.hpp>

namespace EconSimPlusPlus {
    /// An object used to manipulate the view of a scene in OpenGL.
    class Camera {
    private:
        /// The dimensions (width, height in pixels) of the window viewport and therefore the camera viewport.
        glm::vec2 viewport;
        /// The position of the camera in world space.
        glm::vec3 position{};
        /// The projection matrix (e.g., perspective or orthographic).
        glm::mat4 projection;

        /// The point of interest that the camera is "looking at". Used for calculating the view matrix.
        static constexpr glm::vec3 forward{0.0f, 0.0f, -1.0f};
        /// The up direction of the camera. Used for calculating the view matrix.
        static constexpr glm::vec3 up{0.0f, 1.0f, 0.0f};

    public:
        Camera(glm::vec2 viewport_, glm::vec3 position_);

        /// Get the perspective matrix of the camera.
        /// @return A 4x4 matrix.
        [[nodiscard]] glm::mat4 getPerspectiveMatrix() const;

        /// Get the view matrix of the camera.
        /// @return A 4x4 matrix.
        [[nodiscard]] glm::mat4 getViewMatrix() const;

        /// Get the camera position.
        /// @return A 3-vector position in world coordinates.
        [[nodiscard]] glm::vec3 getPosition() const;

        /// Get the viewport size.
        /// @return the width and height in pixels.
        [[nodiscard]] glm::vec2 getViewportSize() const;

        /// Update the camera.
        /// @param deltaTime The size of the step to take in terms of time (seconds).
        /// @param inputState The state of keyboard and mouse input.
        void update(float deltaTime, const InputState& inputState);

        /// Move/translate the camera.
        /// @param direction The direction to move the camera in.
        /// @param speed How fast to move the camera.
        void move(Direction direction, float speed);

        /// Moves the camera back to origin.
        void resetPosition();

        /// Update the camera to match the window size when the user resizes the window.
        /// @param viewport_ The new window size.
        void onWindowResize(glm::vec2 viewport_);

    private:
        /// Generate a projection matrix for the given viewport size.
        /// @param viewport The width and height of the camera view in pixels.
        static glm::mat4 createProjectionMatrix(const glm::vec2& viewport);
    };
} // namespace EconSimPlusPlus


#endif // ECONSIMPLUSPLUS_CAMERA_H
