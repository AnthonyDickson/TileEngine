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

#include "Camera.h"
#include "glm/geometric.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

Camera::Camera(float aspectRatio_, glm::vec3 position_, glm::vec3 forward_, glm::vec3 up_) :
        position{position_}, forward{forward_}, up{up_}, aspectRatio{aspectRatio_} {
    assert(glm::length(forward) == 1.0f && "Forward vector must be normalized.");
    assert(glm::length(up) == 1.0f && "Forward vector must be normalized.");
    assert(glm::dot(forward, up) == 0.0f && "Forward and up vectors must be orthogonal.");
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

void Camera::rotate(const glm::vec2 mouseMovement, const float sensitivity) {
    rotation.x += sensitivity * mouseMovement.y;
    rotation.y += sensitivity * mouseMovement.x;

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

void Camera::zoom(const float scrollDelta) {
    fov -= static_cast<float>(scrollDelta);

    if (fov < 1.0f) {
        fov = 1.0f;
    } else if (fov > 45.0f) {
        fov = 45.0f;
    }
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getDirection() const {
    return forward;
}

glm::mat4 Camera::getPerspectiveMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + forward, up);
}

void Camera::setAspectRatio(float aspectRatio_) {
    aspectRatio = aspectRatio_;
}
