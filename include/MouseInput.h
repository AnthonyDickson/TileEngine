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
// Created by Anthony Dickson on 20/04/2024.
//

#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

class MouseInput {
private:
    /**
     * Whether the mouse position has been set before.
     *
     * This is used to prevent issues with mouse movement in the first update step.
     */
    bool hasInitializedMousePosition{false};
    /** The position of the mouse cursor since the most recent call to `MouseInput::update(...)`. */
    glm::vec2 lastMousePosition{};
    /** How much the mouse has moved since the most recent update step. */
    glm::vec2 mouseMovement{};
    /** The net change in the scroll wheel since the most recent update step. */
    float scrollDelta{};
public:
    /**
     * Poll and update the mouse input state.
     * @param window The window to poll for mouse input.
     */
    void update(GLFWwindow* window);

    /**
     * Update the cumulative scroll wheel movement.
     * @param scrollX The amount of horizontal scroll input.
     * @param scrollY The amount of vertical scroll input.
     */
    void updateScroll(double scrollX, double scrollY);

    /**
     * Reset the scroll wheel movement.
     * Since scroll wheel movement cannot be polled, it must be accumulated between update steps.
     * This function must be called AFTER the main game update step to ensure that the update steps have access to the
     * correct cumulative scroll wheel movement since the last frame.
     */
    void resetScrollDelta();

    /**
     * Get the mouse cursor position.
     * @return the screen coordinates of the mouse cursor in pixels.
     */
    [[nodiscard]] const glm::vec2& getPosition() const;

    /**
     * Get the mouse cursor movement since the last frame.
     * @return the movement of the mouse in pixels.
     */
    [[nodiscard]] const glm::vec2& getMovement() const;
};



#endif //MOUSEINPUT_H
