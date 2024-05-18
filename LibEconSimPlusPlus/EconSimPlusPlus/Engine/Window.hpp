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
// Created by Anthony on 30/03/2024.
//

#ifndef ECONSIMPLUSPLUS_WINDOW_H
#define ECONSIMPLUSPLUS_WINDOW_H

#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"

#include <EconSimPlusPlus/Engine/InputState.hpp>

namespace EconSimPlusPlus::Engine {
    /// Handles the basic functions of an OpenGL window.
    class Window {
    public:
        /// Create and initialize a GLFW window.
        /// @param windowWidth_ The width of the window to create in pixels.
        /// @param windowHeight_ The height of the window to create in pixels.
        /// @param windowName The string to display in the window's title bar.
        Window(int windowWidth_, int windowHeight_, const std::string& windowName);

        Window(Window&) = delete;
        Window(Window&&) = delete;

        /// Releases GLFW window stuff.
        ~Window();

        /// This function should be called before the game's update and draw functions.
        void preUpdate();

        /// This function should be called after the game's update and draw functions.
        void postUpdate();

        /// Whether the application should close.
        [[nodiscard]] bool shouldClose() const;

        /// Get the state of a keyboard key.
        /// @return The state of the given key as an integer (GLFW defined state enum).
        [[nodiscard]] const InputState& inputState() const;

        /// Get the aspect ratio of the window.
        /// @return The aspect ratio in pixels.
        [[nodiscard, maybe_unused]] float apectRatio() const;

        /// Get the width of the window.
        /// @return The width of the window in pixels.
        [[nodiscard]] int width() const;

        /// Get the height of the window.
        /// @return The height of the window in pixels.
        [[nodiscard]] int height() const;

        /// Get the size of the window.
        /// @return The size (width, height) of the window in pixels.
        [[nodiscard]] glm::ivec2 size() const;

        /// Check whether the user has resized the window since the last update step.
        /// @return A bool indicating whether the user has resized the window.
        [[nodiscard]] bool hasWindowSizeChanged() const;

    private:
        /// Handle the window being resized by the user.
        /// @param window A handle to the window.
        /// @param width The new width of the window.
        /// @param height The new height of the window.
        static void onWindowResize(GLFWwindow* window, int width, int height);

        /// Handle the user using the mouse scroll wheel.
        /// @param window A handle to the window.
        /// @param scrollX The amount scrolled horizontally.
        /// @param scrollY The amount scrolled vertically.
        static void onMouseScroll(GLFWwindow* window, double scrollX, double scrollY);

        /// Update the window dimensions.
        /// @param width The width of the window in pixels.
        /// @param height The height of the window in pixels.
        void updateWindowSize(int width, int height);

        /// We only want one instance of `Window`, we use this bool to track whether an instance was already created.
        static bool m_isInitialised;

        /// Handle to the GLFW window object.
        GLFWwindow* m_window{};
        /// The width of the window in pixels.
        int m_windowWidth{};
        /// The height of the window in pixels.
        int m_windowHeight{};
        /// Whether the user changed the window size since the most recent update step.
        bool m_hasWindowChangedSize{false};
        /// Keeps track of keyboard and mouse input.
        InputState m_inputState{};
    };
} // namespace EconSimPlusPlus::Engine

#endif // ECONSIMPLUSPLUS_WINDOW_H
