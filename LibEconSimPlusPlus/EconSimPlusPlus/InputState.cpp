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
    namespace {
        /// The integer codes that map to a valid key code in GLFW.
        constexpr std::array validKeys{
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

        /// The integer codes that map to a valid mouse button code in GLFW.
        constexpr std::array validMouseButtons{
            GLFW_MOUSE_BUTTON_1, GLFW_MOUSE_BUTTON_2, GLFW_MOUSE_BUTTON_3, GLFW_MOUSE_BUTTON_4,
            GLFW_MOUSE_BUTTON_5, GLFW_MOUSE_BUTTON_6, GLFW_MOUSE_BUTTON_7, GLFW_MOUSE_BUTTON_8,
        };
    } // namespace

    void InputState::update(GLFWwindow* window) {
        double xPosition{};
        double yPosition{};
        glfwGetCursorPos(window, &xPosition, &yPosition);

        const glm::vec2 position{static_cast<float>(xPosition), static_cast<float>(yPosition)};

        if (!m_hasInitializedMousePosition) {
            // If the window is created in a position away from the mouse, the distance of the mouse from the window
            // will cause the rotation to be moved by a large amount, likely moving the scene out of view. The below
            // code is executed on the first update to prevent this from happening.
            m_mousePosition = position;
            m_hasInitializedMousePosition = true;
        }

        m_mouseMovement = m_mousePosition - position;
        m_mousePosition = position;

        std::swap(m_currentMouseButtonState, m_previousMouseButtonState);

        for (const auto button : validMouseButtons) {
            m_currentMouseButtonState[button] = glfwGetMouseButton(window, button) == GLFW_PRESS;
        }

        std::swap(m_currentKeyState, m_previousKeyState);

        for (const auto key : validKeys) {
            m_currentKeyState[key] = glfwGetKey(window, key) == GLFW_PRESS;
        }
    }

    void InputState::postUpdate() {
        // Since scroll wheel movement cannot be polled, it must be accumulated between update steps.
        // This function must be called AFTER the main game update step to ensure that the update steps have access to
        // the correct cumulative scroll wheel movement since the last frame.
        m_scrollDelta = 0.0f;
    }

    InputState InputState::withoutKeyboardInput() const {
        auto copy{InputState(*this)};

        copy.m_currentKeyState.fill(false);
        copy.m_previousKeyState.fill(false);

        return copy;
    }

    void InputState::updateScroll(const double, const double scrollY) {
        m_scrollDelta += static_cast<float>(scrollY);
    }

    const glm::vec2& InputState::mousePosition() const {
        return m_mousePosition;
    }

    const glm::vec2& InputState::mouseMovement() const {
        return m_mouseMovement;
    }

    bool InputState::key(const int key) const {
        return m_currentKeyState[key];
    }

    bool InputState::keyDown(const int key) const {
        return this->key(key) and not m_previousKeyState[key];
    }

    bool InputState::keyUp(const int key) const {
        return not this->key(key) and m_previousKeyState[key];
    }

    KeyModifier::KeyModifier InputState::keyModifiers() const {
        auto modifiers = KeyModifier::none;

        if (key(GLFW_KEY_LEFT_CONTROL) or key(GLFW_KEY_RIGHT_CONTROL)) {
            modifiers |= KeyModifier::control;
        }

        if (key(GLFW_KEY_LEFT_SHIFT) or key(GLFW_KEY_RIGHT_SHIFT)) {
            modifiers |= KeyModifier::shift;
        }

        if (key(GLFW_KEY_LEFT_ALT) or key(GLFW_KEY_RIGHT_ALT)) {
            modifiers |= KeyModifier::alt;
        }

        if (key(GLFW_KEY_LEFT_SUPER) or key(GLFW_KEY_RIGHT_SUPER)) {
            modifiers |= KeyModifier::super;
        }

        return modifiers;
    }

    bool InputState::mouseButton(const int button) const {
        return m_currentMouseButtonState[button];
    }

    bool InputState::mouseButtonDown(const int button) const {
        return mouseButton(button) and not m_previousMouseButtonState[button];
    }

    bool InputState::mouseButtonUp(const int button) const {
        return not mouseButton(button) and m_previousMouseButtonState[button];
    }
} // namespace EconSimPlusPlus
