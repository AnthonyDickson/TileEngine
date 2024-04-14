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

#include "Size.h"

/** An object used to manipulate the view of a scene in OpenGL. */
class Camera {
private:
    Size<float> viewport;
    /** The position of the camera in world space. */
    glm::vec3 position{};

    /** The point of interest that the camera is "looking at". Used for calculating the view matrix. */
    static constexpr glm::vec3 center{0.0f, 0.0f, 0.0f};
    /** The up direction of the camera. Used for calculating the view matrix. */
    static constexpr glm::vec3 up{0.0f, 1.0f, 0.0f};
public:
    Camera(Size<float> viewport_, glm::vec3 position_);

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
     * Update the camera to match the window size when the user resizes the window.
     * @param viewport_ The new window size.
     */
    void onWindowResize(Size<float> viewport_);
};


#endif //ECONSIMPLUSPLUS_CAMERA_H
