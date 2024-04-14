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

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "Camera.h"

Camera::Camera(const Size<float> viewport_, const glm::vec3 position_) : viewport(viewport_), position(position_) {}

glm::mat4 Camera::getPerspectiveMatrix() const {
    // Putting window height then 0 causing (0, 0) to start from the top left (i.e. y positive points down).
    return glm::ortho(0.0f, viewport.width, viewport.height, 0.0f, 0.1f, 1000.0f);
}

glm::mat4 Camera::getViewMatrix() const {
    return lookAt(position, center, up);
}

void Camera::onWindowResize(const Size<float> viewport_) {
    viewport = viewport_;
}
