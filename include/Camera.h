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


#include "glm/vec3.hpp"
#include "glm/ext/matrix_float4x4.hpp"

/** An object used to manipulate the view of a scene in OpenGL. */
class Camera {
private:
    /** The position of the camera in world space. */
    glm::vec3 position{};
    /** The direction the camera is pointing in. Must be a normalized vector. */
    glm::vec3 forward{glm::vec3(0.0f, 0.0f, -1.0f)};
    /** The vertical axis of the camera. Must be a normalized vector and perpendicular to the forward vector. */
    glm::vec3 up{glm::vec3(0.0f, 1.0f, 0.0f)};
public:
    /** Direction used for camera movement. */
    enum class Direction {
        forward,
        backward,
        left,
        right
    };

    /**
     * Create a camera.
     * @param position Where the camera should initially be positioned.
     * @param forward The forward direction for the camera. Note: Must be a normalized vector and orthogonal with `up`.
     * @param up The up direction for the camera. Note: Must be a normalized vector and orthogonal with `forward`.
     */
    explicit Camera(glm::vec3 position = glm::vec3{},
                    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f),
                    glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f});

    /**
     * Move the camera in a given direction.
     * @param direction The direction to move the camera.
     * @param speed How fast to move the camera.
     */
    void move(Direction direction, float speed = 1.0f);

    /**
     * Get the look at matrix for the camera.
     * @return A 4x4 matrix.
     */
    [[nodiscard]] glm::mat4 getLookAtMatrix() const;
};


#endif //ECONSIMPLUSPLUS_CAMERA_H
