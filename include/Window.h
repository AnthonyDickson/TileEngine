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

#include "Camera.h"
#include "InputState.h"
#include "Size.h"

/** Handles the basic functions of an OpenGL window. */
class Window {
private:
    /** Handle to the GLFW window object. */
    GLFWwindow* window{};
    /** The width of the window in pixels. */
    int windowWidth{};
    /** The height of the window in pixels. */
    int windowHeight{};
    /** Whether the user changed the window size since the most recent update step. */
    bool hasWindowChangedSize{false};

    /** Keeps track of keyboard and mouse input. */
    InputState inputState{};

    /** We only want one instance of `Window`, we use this bool to track whether an instance was already created. */
    static bool isInitialised;

public:
    /**
     * Create and initialize a GLFW window.
     * @param windowWidth_ The width of the window to create in pixels.
     * @param windowHeight_ The height of the window to create in pixels.
     * @param windowName The string to display in the window's title bar.
     */
    Window(int windowWidth_, int windowHeight_, const std::string& windowName);

    Window(Window&) = delete;
    Window(Window&&) = delete;

    /**
     * Releases GLFW window stuff.
     */
    ~Window();

    /**
     * This function should be called before the game's update function.
     */
    void preUpdate();

    /**
     * This function should be called after the game's update function.
     */
    void postUpdate();

    /**
     * Signal that the application should close. This will happen at the start of the next iteration of the main loop.
     */
    void close() const;

    /**
     * Get the state of a keyboard key.
     * @return The state of the given key as an integer (GLFW defined state enum).
     */
    [[nodiscard]] const InputState& getInputState() const;

    /**
     * The net change in the vertical scroll wheel since the last update step.
     * @return The net scroll amount.
     */
    [[nodiscard, maybe_unused]] float getMouseScroll() const;

    /**
     * Get the screen coordinates of the mouse cursor.
     * @return A 2-vector.
     */
    [[nodiscard, maybe_unused]] glm::vec2 getMousePosition() const;

    /**
     * Get the distance the mouse has moved since the last update step measured in pixels.
     * @return A 2-vector.
     */
    [[nodiscard, maybe_unused]] glm::vec2 getMouseDelta() const;

    /**
     * Get the aspect ratio of the window.
     * @return The aspect ratio in pixels.
     */
    [[nodiscard, maybe_unused]] float getAspectRatio() const;

    /**
     * Get the width of the window.
     * @return The width of the window in pixels.
     */
    [[nodiscard]] int getWidth() const;

    /**
     * Get the height of the window.
     * @return The height of the window in pixels.
     */
    [[nodiscard]] int getHeight() const;

    /**
     * Get the size of the window.
     * @return The size (width, height) of the window in pixels.
     */
    [[nodiscard]] Size<int> getSize() const;

    /**
     * Check whether the user has resized the window since the last update step.
     * @return A bool indicating whether the user has resized the window.
     */
    [[nodiscard]] bool hasWindowSizeChanged() const;

private:
    /**
     * Handle the window being resized by the user.
     * @param window A handle to the window.
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    static void onWindowResize(GLFWwindow* window, int width, int height);

    /**
     * Handle the user using the mouse scroll wheel.
     * @param window A handle to the window.
     * @param scrollX The amount scrolled horizontally.
     * @param scrollY The amount scrolled vertically.
     */
    static void onMouseScroll(GLFWwindow* window, double scrollX, double scrollY);

    /**
     * Update the window dimensions.
     * @param width The width of the window in pixels.
     * @param height The height of the window in pixels.
     */
    void updateWindowSize(int width, int height);
};


#endif // ECONSIMPLUSPLUS_WINDOW_H
