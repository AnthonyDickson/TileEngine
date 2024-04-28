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

#include <array>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

namespace EconSimPlusPlus {
    /// Keeps track of keyboard and mouse input.
    class InputState {
        /// Implicit mapping between GLFW key codes and whether the key is pressed down in this frame.
        std::array<bool, GLFW_KEY_LAST + 1> currentKeyState{};
        /// Implicit mapping between GLFW key codes and whether the key is pressed down in the previous frame.
        std::array<bool, GLFW_KEY_LAST + 1> previousKeyState{};

        /// The integer codes that map to a valid key code in GLFW.
        constexpr static std::array validKeys{
            GLFW_KEY_SPACE,
            GLFW_KEY_APOSTROPHE,
            GLFW_KEY_COMMA,
            GLFW_KEY_MINUS,
            GLFW_KEY_PERIOD,
            GLFW_KEY_SLASH,
            GLFW_KEY_0,
            GLFW_KEY_1,
            GLFW_KEY_2,
            GLFW_KEY_3,
            GLFW_KEY_4,
            GLFW_KEY_5,
            GLFW_KEY_6,
            GLFW_KEY_7,
            GLFW_KEY_8,
            GLFW_KEY_9,
            GLFW_KEY_SEMICOLON,
            GLFW_KEY_EQUAL,
            GLFW_KEY_A,
            GLFW_KEY_B,
            GLFW_KEY_C,
            GLFW_KEY_D,
            GLFW_KEY_E,
            GLFW_KEY_F,
            GLFW_KEY_G,
            GLFW_KEY_H,
            GLFW_KEY_I,
            GLFW_KEY_J,
            GLFW_KEY_K,
            GLFW_KEY_L,
            GLFW_KEY_M,
            GLFW_KEY_N,
            GLFW_KEY_O,
            GLFW_KEY_P,
            GLFW_KEY_Q,
            GLFW_KEY_R,
            GLFW_KEY_S,
            GLFW_KEY_T,
            GLFW_KEY_U,
            GLFW_KEY_V,
            GLFW_KEY_W,
            GLFW_KEY_X,
            GLFW_KEY_Y,
            GLFW_KEY_Z,
            GLFW_KEY_LEFT_BRACKET,
            GLFW_KEY_BACKSLASH,
            GLFW_KEY_RIGHT_BRACKET,
            GLFW_KEY_GRAVE_ACCENT,
            GLFW_KEY_WORLD_1,
            GLFW_KEY_WORLD_2,
            GLFW_KEY_ESCAPE,
            GLFW_KEY_ENTER,
            GLFW_KEY_TAB,
            GLFW_KEY_BACKSPACE,
            GLFW_KEY_INSERT,
            GLFW_KEY_DELETE,
            GLFW_KEY_RIGHT,
            GLFW_KEY_LEFT,
            GLFW_KEY_DOWN,
            GLFW_KEY_UP,
            GLFW_KEY_PAGE_UP,
            GLFW_KEY_PAGE_DOWN,
            GLFW_KEY_HOME,
            GLFW_KEY_END,
            GLFW_KEY_CAPS_LOCK,
            GLFW_KEY_SCROLL_LOCK,
            GLFW_KEY_NUM_LOCK,
            GLFW_KEY_PRINT_SCREEN,
            GLFW_KEY_PAUSE,
            GLFW_KEY_F1,
            GLFW_KEY_F2,
            GLFW_KEY_F3,
            GLFW_KEY_F4,
            GLFW_KEY_F5,
            GLFW_KEY_F6,
            GLFW_KEY_F7,
            GLFW_KEY_F8,
            GLFW_KEY_F9,
            GLFW_KEY_F10,
            GLFW_KEY_F11,
            GLFW_KEY_F12,
            GLFW_KEY_F13,
            GLFW_KEY_F14,
            GLFW_KEY_F15,
            GLFW_KEY_F16,
            GLFW_KEY_F17,
            GLFW_KEY_F18,
            GLFW_KEY_F19,
            GLFW_KEY_F20,
            GLFW_KEY_F21,
            GLFW_KEY_F22,
            GLFW_KEY_F23,
            GLFW_KEY_F24,
            GLFW_KEY_F25,
            GLFW_KEY_KP_0,
            GLFW_KEY_KP_1,
            GLFW_KEY_KP_2,
            GLFW_KEY_KP_3,
            GLFW_KEY_KP_4,
            GLFW_KEY_KP_5,
            GLFW_KEY_KP_6,
            GLFW_KEY_KP_7,
            GLFW_KEY_KP_8,
            GLFW_KEY_KP_9,
            GLFW_KEY_KP_DECIMAL,
            GLFW_KEY_KP_DIVIDE,
            GLFW_KEY_KP_MULTIPLY,
            GLFW_KEY_KP_SUBTRACT,
            GLFW_KEY_KP_ADD,
            GLFW_KEY_KP_ENTER,
            GLFW_KEY_KP_EQUAL,
            GLFW_KEY_LEFT_SHIFT,
            GLFW_KEY_LEFT_CONTROL,
            GLFW_KEY_LEFT_ALT,
            GLFW_KEY_LEFT_SUPER,
            GLFW_KEY_RIGHT_SHIFT,
            GLFW_KEY_RIGHT_CONTROL,
            GLFW_KEY_RIGHT_ALT,
            GLFW_KEY_RIGHT_SUPER,
            GLFW_KEY_MENU,
        };

        /// Implicit mapping between GLFW key codes and whether the mouse button is pressed down in this frame.
        std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> currentMouseButtonState{};
        /// Implicit mapping between GLFW key codes and whether the mouse button is pressed down in the previous frame.
        std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> previousMouseButtonState{};

        /// The integer codes that map to a valid mouse button code in GLFW.
        constexpr static std::array validMouseButtons{
            GLFW_MOUSE_BUTTON_1, GLFW_MOUSE_BUTTON_2, GLFW_MOUSE_BUTTON_3, GLFW_MOUSE_BUTTON_4,
            GLFW_MOUSE_BUTTON_5, GLFW_MOUSE_BUTTON_6, GLFW_MOUSE_BUTTON_7, GLFW_MOUSE_BUTTON_8,
        };

        /// Whether the mouse position has been set before.
        /// This is used to prevent issues with mouse movement in the first update step.
        bool hasInitializedMousePosition{false};
        /// The position of the mouse cursor since the most recent call to `InputState::update(...)`.
        glm::vec2 mousePosition{};
        /// How much the mouse has moved since the most recent update step.
        glm::vec2 mouseMovement{};
        /// The net change in the scroll wheel since the most recent update step.
        float scrollDelta{};

    public:
        /// Poll and update the keyboard and mouse input state.
        /// @param window The GLFW window get keyboard and mouse input from.
        void update(GLFWwindow* window);

        /// Perform any actions necessary for the post-update step.
        void postUpdate();

        /// Check whether a key is pressed down.
        /// @param key A GLFW key code.
        /// @return `true` if the key is pressed down, otherwise `false`.
        [[nodiscard]] bool getKey(int key) const;

        /// Check whether a key was pressed during this frame.
        /// @param key A GLFW key code.
        /// @return `true` if the key was pressed down during this frame, otherwise `false`.
        [[nodiscard]] bool getKeyDown(int key) const;

         /// Check whether a key was released during this frame.
         /// @param key A GLFW key code.
         /// @return `true` if the key was released during this frame, otherwise `false`.
        [[nodiscard]] bool getKeyUp(int key) const;

         /// Get the position of the mouse cursor.
         /// @return the screen coordinates of the mouse cursor in pixels.
        [[nodiscard]] const glm::vec2& getMousePosition() const;

         /// Get the mouse cursor movement since the last frame.
         /// @return the movement of the mouse in pixels.
        [[nodiscard]] const glm::vec2& getMouseMovement() const;

         /// Update the cumulative scroll wheel movement.
         /// @param scrollX The amount of horizontal scroll input.
         /// @param scrollY The amount of vertical scroll input.
        void updateScroll(double scrollX, double scrollY);

         /// Check whether a mouse button is pressed down.
         /// @param button A GLFW mouse button code.
         /// @return `true` if the mouse button is pressed down, otherwise `false`.
        [[nodiscard]] bool getMouseButton(int button) const;

         /// Check whether a mouse button was pressed during this frame.
         /// @param button A GLFW mouse button code.
         /// @return `true` if the mouse button was pressed down during this frame, otherwise `false`.
        [[nodiscard]] bool getMouseButtonDown(int button) const;

         /// Check whether a mouse button was released during this frame.
         /// @param button A GLFW mouse button code.
         /// @return `true` if the mouse button was released during this frame, otherwise `false`.
        [[nodiscard]] bool getMouseButtonUp(int button) const;
    };
} // namespace EconSimPlusPlus

#endif // INPUTSTATE_H
