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

#include <stdexcept>
#include "Camera.h"
#include "glm/geometric.hpp"
#include "glm/ext/matrix_transform.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up) : position{position}, forward{forward}, up{up} {
    if (glm::dot(forward, up) != 0.0f) {
        throw std::logic_error{"Forward and up vectors are not orthogonal."};
    }

    if (glm::length(forward) != 1.0f) {
        throw std::logic_error{"Forward vector is not normalized (length != 1)."};
    }

    if (glm::length(up) != 1.0f) {
        throw std::logic_error{"Forward vector is not normalized (length != 1)."};
    }
}

void Camera::move(Camera::Direction direction, float speed) {
    switch (direction) {
        case Direction::forward:
            position += speed * forward;
            return;
        case Direction::backward:
            position -= speed * forward;
            return;
        case Direction::left:
            position -= speed * glm::normalize(glm::cross(forward, up));
            return;
        case Direction::right:
            position += speed * glm::normalize(glm::cross(forward, up));
            return;
    }
}

void Camera::updateRotation(const glm::vec2 mousePosition) {
    if (!hasInitialisedMousePosition) {
        // If the window is created in a position away from the mouse, the distance of the mouse from the window will
        // cause the rotation to be moved by a large amount, likely moving the scene out of view.
        // The below code is executed on the first update to prevent this from happening.
        lastMousePosition = mousePosition;
        hasInitialisedMousePosition = true;
    }

    glm::vec2 mouseMovement{mousePosition.x - lastMousePosition.x, lastMousePosition.y - mousePosition.y};
    lastMousePosition = mousePosition;

    const float sensitivity = 0.1f;
    mouseMovement *= sensitivity;

    rotation.x += mouseMovement.y;
    rotation.y += mouseMovement.x;

    if (rotation.x > 89.0f) {
        rotation.x = 89.0f;
    } else if (rotation.x < -89.0f) {
        rotation.x = -89.0f;
    }

    glm::vec3 direction{};
    direction.x = static_cast<float>(cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)));
    direction.y = static_cast<float>(sin(glm::radians(rotation.x)));
    direction.z = static_cast<float>(cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y)));
    forward = glm::normalize(direction);
}

glm::mat4 Camera::getLookAtMatrix() const {
    return glm::lookAt(position, position + forward, up);
}
