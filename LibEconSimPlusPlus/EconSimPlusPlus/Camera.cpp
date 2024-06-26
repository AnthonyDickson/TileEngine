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

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Camera.hpp>

namespace EconSimPlusPlus {
    namespace {
        /// The point of interest that the camera is "looking at". Used for calculating the view matrix.
        constexpr glm::vec3 forward{0.0f, 0.0f, -1.0f};
        /// The up direction of the camera. Used for calculating the view matrix.
        constexpr glm::vec3 up{0.0f, 1.0f, 0.0f};

        /// Generate a projection matrix for the given viewport size.
        /// @param viewport The width and height of the camera view in pixels.
        glm::mat4 createProjectionMatrix(const glm::vec2& viewport) {
            return glm::ortho(-viewport.x / 2.0f, viewport.x / 2.0f, -viewport.y / 2.0f, viewport.y / 2.0f, 0.1f,
                              1000.0f);
        }
    } // namespace

    Camera::Camera(const glm::vec2 viewport, const glm::vec3 position) :
        m_viewport(viewport), m_position(position), m_projection(createProjectionMatrix(viewport)) {
    }

    glm::mat4 Camera::perspectiveMatrix() const {
        return m_projection;
    }

    glm::mat4 Camera::viewMatrix() const {
        return lookAt(m_position, forward + m_position, up);
    }

    glm::vec3 Camera::position() const {
        return m_position;
    }

    glm::vec2 Camera::viewportSize() const {
        return m_viewport;
    }

    Viewport Camera::viewport() const {
        const glm::vec2 position2D{m_position.x, m_position.y};

        return {position2D - 0.5f * m_viewport, position2D + 0.5f * m_viewport};
    }

    void Camera::update(const float deltaTime, const InputState& inputState) {
        constexpr float speed = 512.0f;

        if (inputState.key(GLFW_KEY_W)) {
            move(Direction::Up, deltaTime * speed);
        }

        if (inputState.key(GLFW_KEY_S)) {
            move(Direction::Down, deltaTime * speed);
        }

        if (inputState.key(GLFW_KEY_A)) {
            move(Direction::Left, deltaTime * speed);
        }

        if (inputState.key(GLFW_KEY_D)) {
            move(Direction::Right, deltaTime * speed);
        }

        const glm::vec2& mouseMovement{inputState.mouseMovement()};

        if (inputState.mouseButton(GLFW_MOUSE_BUTTON_MIDDLE) or inputState.key(GLFW_KEY_LEFT_ALT)) {
            if (mouseMovement.x < 0.0f) {
                move(Direction::Left, std::abs(mouseMovement.x));
            }
            else {
                move(Direction::Right, std::abs(mouseMovement.x));
            }

            if (mouseMovement.y < 0.0f) {
                move(Direction::Up, std::abs(mouseMovement.y));
            }
            else {
                move(Direction::Down, std::abs(mouseMovement.y));
            }
        }

        if (inputState.keyDown(GLFW_KEY_C)) {
            resetPosition();
        }
    }

    void Camera::move(const Direction direction, const float speed) {
        switch (direction) {
        case Direction::Up:
            m_position.y += speed;
            break;
        case Direction::Down:
            m_position.y -= speed;
            break;
        case Direction::Left:
            m_position.x -= speed;
            break;
        case Direction::Right:
            m_position.x += speed;
            break;
        }
    }

    void Camera::resetPosition() {
        m_position.x = 0.0f;
        m_position.y = 0.0f;
    }

    void Camera::onWindowResize(const glm::vec2 viewport_) {
        m_viewport = viewport_;
        m_projection = createProjectionMatrix(viewport_);
    }

    glm::vec2 screenToWorldCoordinates(const glm::vec2 screenCoordinates, const Camera& camera) {
        const auto [bottomLeft, topRight]{camera.viewport()};
        return {screenCoordinates.x + bottomLeft.x, -screenCoordinates.y + topRight.y};
    }

} // namespace EconSimPlusPlus
