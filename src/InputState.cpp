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

#include <EconSimPlusPlus/InputState.hpp>

namespace EconSimPlusPlus {
    void InputState::update(GLFWwindow* window) {
        double xPosition{};
        double yPosition{};
        glfwGetCursorPos(window, &xPosition, &yPosition);

        const glm::vec2 position{static_cast<float>(xPosition), static_cast<float>(yPosition)};

        if (!hasInitializedMousePosition) {
            // If the window is created in a position away from the mouse, the distance of the mouse from the window
            // will cause the rotation to be moved by a large amount, likely moving the scene out of view. The below
            // code is executed on the first update to prevent this from happening.
            mousePosition = position;
            hasInitializedMousePosition = true;
        }

        mouseMovement.x = position.x - mousePosition.x;
        mouseMovement.y = mousePosition.y - position.y;
        mousePosition = position;

        std::swap(currentMouseButtonState, previousMouseButtonState);

        for (const auto button : validMouseButtons) {
            currentMouseButtonState[button] = glfwGetMouseButton(window, button) == GLFW_PRESS;
        }

        std::swap(currentKeyState, previousKeyState);

        for (const auto key : validKeys) {
            currentKeyState[key] = glfwGetKey(window, key) == GLFW_PRESS;
        }
    }

    void InputState::postUpdate() {
        // Since scroll wheel movement cannot be polled, it must be accumulated between update steps.
        // This function must be called AFTER the main game update step to ensure that the update steps have access to
        // the correct cumulative scroll wheel movement since the last frame.
        scrollDelta = 0.0f;
    }

    void InputState::updateScroll(const double, const double scrollY) {
        scrollDelta += static_cast<float>(scrollY);
    }

    const glm::vec2& InputState::getMousePosition() const {
        return mousePosition;
    }

    const glm::vec2& InputState::getMouseMovement() const {
        return mouseMovement;
    }

    bool InputState::getKey(const int key) const {
        return currentKeyState[key];
    }

    bool InputState::getKeyDown(const int key) const {
        return currentKeyState[key] and !previousKeyState[key];
    }

    bool InputState::getKeyUp(const int key) const {
        return !currentKeyState[key] and previousKeyState[key];
    }

    bool InputState::getMouseButton(const int button) const {
        return currentMouseButtonState[button];
    }

    bool InputState::getMouseButtonDown(const int button) const {
        return currentMouseButtonState[button] and !previousMouseButtonState[button];
    }

    bool InputState::getMouseButtonUp(const int button) const {
        return !currentMouseButtonState[button] and previousMouseButtonState[button];
    }
} // namespace EconSimPlusPlus
