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
# include <iostream>

#include "glad/glad.h"

#include "Window.h"

Window::Window(int windowWidth_, int windowHeight_, const std::string &windowName) : windowWidth(windowWidth_),
                                                                                     windowHeight(windowHeight_) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), nullptr, nullptr);

    if (window == nullptr) {
        throw std::runtime_error("Failed to create the GLFW window.");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetWindowSizeCallback(window, Window::onWindowResize);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, Window::onMouseScroll);
    glfwSetWindowUserPointer(window, this);
}

Window::~Window() {
    glfwTerminate();
}

void Window::runMainLoop(const std::function<void(float)> &updateFunction) {
    lastFrameTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window)) {
        const auto currentFrameTime = static_cast<float>(glfwGetTime());
        const auto deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        updateMousePosition();

        updateFunction(deltaTime);
        // scrollDelta should be reset AFTER the update function is called, otherwise the update function will always
        // see a zero value for scrollDelta.
        scrollDelta = 0.0f;
        hasWindowChangedSize = false;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::close() {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int Window::getKeyState(int key) {
    return glfwGetKey(window, key);
}

float Window::getMouseScroll() const {
    return scrollDelta;
}

glm::vec2 Window::getMousePosition() {
    return lastMousePosition;
}

glm::vec2 Window::getMouseDelta() {
    return mouseMovement;
}

float Window::getAspectRatio() const {
    return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

bool Window::hasWindowSizeChanged() const {
    return hasWindowChangedSize;
}

void Window::onWindowResize(GLFWwindow *window_, int width, int height) {
    auto windowHandle{reinterpret_cast<Window *>(glfwGetWindowUserPointer(window_))};

    if (windowHandle) {
        windowHandle->windowWidth = width;
        windowHandle->windowHeight = height;
        glViewport(0, 0, windowHandle->windowWidth, windowHandle->windowHeight);
        windowHandle->hasWindowChangedSize = true;
    }
}

void Window::onMouseScroll(GLFWwindow *window, double, double scrollY) {
    auto windowHandle{reinterpret_cast<Window *>(glfwGetWindowUserPointer(window))};

    if (windowHandle) {
        windowHandle->scrollDelta += static_cast<float>(scrollY);
    }
}

void Window::updateMousePosition() {
    double xPosition{};
    double yPosition{};
    glfwGetCursorPos(window, &xPosition, &yPosition);

    glm::vec2 position{static_cast<float>(xPosition), static_cast<float>(yPosition)};

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
