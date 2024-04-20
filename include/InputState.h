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

#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include "KeyboardInput.h"
#include "MouseInput.h"

class InputState {
    /** Keeps track of keyboard and mouse input. */
private:
    /** Keeps track of keyboard input. */
    KeyboardInput keyboardInput{};
    /** Keeps track of mouse input. */
    MouseInput mouseInput{};

public:
    /**
     * Poll and update the keyboard and mouse input state.
     * @param window The GLFW window get keyboard and mouse input from.
     */
    void update(GLFWwindow* window);

    /**
     * Perform any actions necessary for the post-update step.
     */
    void postUpdate();

    /**
     * Check whether a key is currently released.
     * @param key A GLFW key code.
     * @return `true` if the key is currently released, otherwise `false`.
     */
    [[nodiscard]] bool isKeyUp(int key) const;

    /**
     * Check whether a key is currently pressed down.
     * @param key A GLFW key code.
     * @return `true` if the key is currently pressed down, otherwise `false`.
     */
    [[nodiscard]] bool isKeyDown(int key) const;

    /**
     * Get the position of the mouse cursor.
     * @return the screen coordinates of the mouse cursor in pixels.
     */
    [[nodiscard]] glm::vec2 getMousePosition() const;

    /**
     * Get the mouse cursor movement since the last frame.
     * @return the movement of the mouse in pixels.
     */
    [[nodiscard]] glm::vec2 getMouseMovement() const;

    /**
     * Update the cumulative scroll wheel movement.
     * @param scrollX The amount of horizontal scroll input.
     * @param scrollY The amount of vertical scroll input.
     */
    void updateScroll(double scrollX, double scrollY);
};



#endif //INPUTSTATE_H
