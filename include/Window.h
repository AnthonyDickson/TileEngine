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

#include <functional>

#include "GLFW/glfw3.h"

#include "Camera.h"

/** Handles the basic functions of an OpenGL window. */
class Window {
private:
    /** Handle to the GLFW window object. */
    GLFWwindow *window{};
    /** The width of the window in pixels. */
    int windowWidth{};
    /** The height of the window in pixels. */
    int windowHeight{};
    /** For tracking time between frames (seconds). */
    float lastFrameTime{};

    bool hasInitializedMousePosition{false};
    glm::vec2 lastMousePosition{};
    glm::vec2 mouseMovement{};
    float scrollDelta{};
public:
    Window(int windowWidth_, int windowHeight_);

    ~Window();

    void runMainLoop(const std::function<void(float)> &updateFunction);

    void close();

    [[nodiscard]] int getKeyState(int key);

    [[nodiscard]] float getMouseScroll() const;

    [[nodiscard]] glm::vec2 getMousePosition();

    [[nodiscard]] glm::vec2 getMouseDelta();

    [[nodiscard]] float getAspectRatio() const;

private:
    static void onMouseScroll(GLFWwindow *, double scrollX, double);

    void updateMousePosition();
};


#endif //ECONSIMPLUSPLUS_WINDOW_H
