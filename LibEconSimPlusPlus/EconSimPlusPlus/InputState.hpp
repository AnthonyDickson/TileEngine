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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_INPUTSTATE_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_INPUTSTATE_HPP

#include <array>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#include <EconSimPlusPlus/KeyModifier.hpp>

namespace EconSimPlusPlus {
    /// Keeps track of keyboard and mouse input.
    class InputState {
    public:
        /// Poll and update the keyboard and mouse input state.
        /// @param window The GLFW window get keyboard and mouse input from.
        void update(GLFWwindow* window);

        /// Perform any actions necessary for the post-update step.
        void postUpdate();

        /// Create a copy of the input state where all keys are set to the released state.
        /// @return A new `InputState` object.
        [[nodiscard]] InputState withoutKeyboardInput() const;

        /// Check whether a key is pressed down.
        /// @param key A GLFW key code.
        /// @return `true` if the key is pressed down, otherwise `false`.
        [[nodiscard]] bool key(int key) const;

        /// Check whether a key was pressed during this frame.
        /// @param key A GLFW key code.
        /// @return `true` if the key was pressed down during this frame, otherwise `false`.
        [[nodiscard]] bool keyDown(int key) const;

        /// Check whether a key was released during this frame.
        /// @param key A GLFW key code.
        /// @return `true` if the key was released during this frame, otherwise `false`.
        [[nodiscard]] bool keyUp(int key) const;

        /// Get the active key modifiers (control, shift, alt and super) during this frame.
        /// @return The active key modifiers.
        [[nodiscard]] KeyModifier::KeyModifier keyModifiers() const;

        /// Get the position of the mouse cursor.
        /// @return the screen coordinates of the mouse cursor in pixels.
        [[nodiscard]] const glm::vec2& mousePosition() const;

        /// Get the mouse cursor movement since the last frame.
        /// @return the movement of the mouse in pixels.
        [[nodiscard]] const glm::vec2& mouseMovement() const;

        /// Update the cumulative scroll wheel movement.
        /// @param scrollX The amount of horizontal scroll input.
        /// @param scrollY The amount of vertical scroll input.
        void updateScroll(double scrollX, double scrollY);

        /// Check whether a mouse button is pressed down.
        /// @param button A GLFW mouse button code.
        /// @return `true` if the mouse button is pressed down, otherwise `false`.
        [[nodiscard]] bool mouseButton(int button) const;

        /// Check whether a mouse button was pressed during this frame.
        /// @param button A GLFW mouse button code.
        /// @return `true` if the mouse button was pressed down during this frame, otherwise `false`.
        [[nodiscard]] bool mouseButtonDown(int button) const;

        /// Check whether a mouse button was released during this frame.
        /// @param button A GLFW mouse button code.
        /// @return `true` if the mouse button was released during this frame, otherwise `false`.
        [[nodiscard]] bool mouseButtonUp(int button) const;

    private:
        /// Implicit mapping between GLFW key codes and whether the key is pressed down in this frame.
        std::array<bool, GLFW_KEY_LAST + 1> m_currentKeyState{};
        /// Implicit mapping between GLFW key codes and whether the key is pressed down in the previous frame.
        std::array<bool, GLFW_KEY_LAST + 1> m_previousKeyState{};

        /// Implicit mapping between GLFW key codes and whether the mouse button is pressed down in this frame.
        std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> m_currentMouseButtonState{};
        /// Implicit mapping between GLFW key codes and whether the mouse button is pressed down in the previous frame.
        std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> m_previousMouseButtonState{};

        /// Whether the mouse position has been set before.
        /// This is used to prevent issues with mouse movement in the first update step.
        bool m_hasInitializedMousePosition{false};
        /// The position of the mouse cursor since the most recent call to `InputState::update(...)`.
        glm::vec2 m_mousePosition{};
        /// How much the mouse has moved since the most recent update step.
        glm::vec2 m_mouseMovement{};
        /// The net change in the scroll wheel since the most recent update step.
        float m_scrollDelta{};
    };
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_INPUTSTATE_HPP
