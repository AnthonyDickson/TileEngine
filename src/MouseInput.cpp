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

#include "MouseInput.h"

void MouseInput::update(GLFWwindow* window) {
    double xPosition{};
    double yPosition{};
    glfwGetCursorPos(window, &xPosition, &yPosition);

    const glm::vec2 position{static_cast<float>(xPosition), static_cast<float>(yPosition)};

    if (!hasInitializedMousePosition) {
        // If the window is created in a position away from the mouse, the distance of the mouse from the window will
        // cause the rotation to be moved by a large amount, likely moving the scene out of view.
        // The below code is executed on the first update to prevent this from happening.
        lastMousePosition = position;
        hasInitializedMousePosition = true;
    }

    mouseMovement.x = position.x - lastMousePosition.x;
    mouseMovement.y = lastMousePosition.y - position.y;
    lastMousePosition = position;
}

void MouseInput::updateScroll(const double, const double scrollY) {
    scrollDelta += static_cast<float>(scrollY);
}

void MouseInput::resetScrollDelta() {
    scrollDelta = 0.0f;
}

const glm::vec2& MouseInput::getPosition() const {
    return lastMousePosition;
}

const glm::vec2& MouseInput::getMovement() const {
    return mouseMovement;
}

