

//
// Created by Anthony on 30/03/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_WINDOW_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_WINDOW_HPP

// ReSharper disable once CppUnusedIncludeDirective
#include <string>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <EconSimPlusPlus/InputState.hpp>

namespace EconSimPlusPlus {
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

        /// Set the window's cursor display.
        /// @param standardCursorType A standard cursor type as defined by GLFW.
        void setCursor(int standardCursorType);

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

        GLFWcursor* m_cursor{};
    };

    /// Get the coordinate for the top left corner of the window.
    /// @param window A window.
    /// @return 2D camera space coordinates of the top left corner.
    [[nodiscard]] glm::vec2 topLeft(const Window& window);

    /// Get the coordinate for the top right corner of the window.
    /// @param window A window.
    /// @return 2D camera space coordinates of the top right corner.
    [[nodiscard]] glm::vec2 topRight(const Window& window);
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_WINDOW_HPP
