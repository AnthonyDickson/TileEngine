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
#include "GLFW/glfw3.h"

/** An object used to manipulate the view of a scene in OpenGL. */
class Camera {
private:
    /** The position of the camera in world space. */
    glm::vec3 position{};
    /** The direction the camera is pointing in. Must be a normalized vector. */
    glm::vec3 forward{glm::vec3(0.0f, 0.0f, -1.0f)};
    /** The vertical axis of the camera. Must be a normalized vector and perpendicular to the forward vector. */
    glm::vec3 up{glm::vec3(0.0f, 1.0f, 0.0f)};

    /** The vertical field of view (degrees). */
    float fov{45.0f};
    /** The ratio between the width and height of the camera viewport. */
    float aspectRatio{1.0f};
    /** The rotation in Euler angles (xyz, degrees). */
    glm::vec3 rotation{0.f, -90.0f, 0.0f}; // Zero y-axis rotation points to the right, -90deg points forward.
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
     * @param aspectRatio_ The aspect ratio of the render window.
     * @param position_ Where the camera should initially be positioned.
     * @param forward_ The forward direction for the camera. Note: Must be a normalized vector and orthogonal with `up`.
     * @param up_ The up direction for the camera. Note: Must be a normalized vector and orthogonal with `forward`.
     */
    explicit Camera(float aspectRatio_,
                    glm::vec3 position_ = glm::vec3{},
                    glm::vec3 forward_ = glm::vec3(0.0f, 0.0f, -1.0f),
                    glm::vec3 up_ = glm::vec3{0.0f, 1.0f, 0.0f});

    /**
     * Move the camera in a given direction.
     * @param direction The direction to move the camera.
     * @param speed How fast to move the camera.
     */
    void move(Direction direction, float speed = 1.0f);

    /**
     * Rotate the camera based on mouse movement.
     *
     * @param mouseMovement The new coordinates of the mouse pointer.
     * @param sensitivity How sensitive the camera should be to the mouse movement. Scales `mouseMovement`.
     */
    void rotate(glm::vec2 mouseMovement, float sensitivity = 0.1f);

    /**
     * Zoom the camera based on mouse scroll.
     * @param scrollDelta The amount scrolled vertically.
     */
    void zoom(float scrollDelta);

    /**
     * Get the camera's position.
     * @return A 3-vector.
     */
    [[nodiscard]] glm::vec3 getPosition() const;

    /**
     * Get the camera's forward vector.
     * @return A 3-vector.
     */
    [[nodiscard]] glm::vec3 getDirection() const;

    /**
     * Get the perspective matrix of the camera.
     * @return A 4x4 matrix.
     */
    [[nodiscard]] glm::mat4 getPerspectiveMatrix() const;

    /**
     * Get the view matrix of the camera.
     * @return A 4x4 matrix.
     */
    [[nodiscard]] glm::mat4 getViewMatrix() const;

    /**
     * Update the aspect ratio, typically after the user changes the application window size.
     * @param aspectRatio_ The ratio of the window width divided by the window height.
     */
    void setAspectRatio(float aspectRatio_);
};


#endif //ECONSIMPLUSPLUS_CAMERA_H
