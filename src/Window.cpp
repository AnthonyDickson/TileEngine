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

Window::Window(const int windowWidth_, const int windowHeight_, const std::string &windowName)
    : windowWidth(windowWidth_), windowHeight(windowHeight_) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), nullptr, nullptr);

    if (window == nullptr) {
        throw std::runtime_error("Failed to create the GLFW window.");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to initialize GLAD.");
    }

    updateWindowSize(windowWidth_, windowHeight_);
    glfwSetWindowSizeCallback(window, onWindowResize);
    glfwSetScrollCallback(window, onMouseScroll);
    glfwSetWindowUserPointer(window, this);
    glfwSwapInterval(0);  // Let the game handle vsync
}

Window::~Window() {
    glfwTerminate();
}

void Window::preUpdate() {
    updateMousePosition();
}

void Window::postUpdate() {
    // scrollDelta should be reset AFTER the update function is called, otherwise the update function will always
    // see a zero value for scrollDelta.
    scrollDelta = 0.0f;
    hasWindowChangedSize = false;

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::close() const {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int Window::getKeyState(const int key) const {
    return glfwGetKey(window, key);
}

float Window::getMouseScroll() const {
    return scrollDelta;
}

glm::vec2 Window::getMousePosition() const {
    return lastMousePosition;
}

glm::vec2 Window::getMouseDelta() const {
    return mouseMovement;
}

int Window::getWidth() const {
    return windowWidth;
}

int Window::getHeight() const {
    return windowHeight;
}

Size<int> Window::getSize() const {
    return {windowWidth, windowHeight};
}

float Window::getAspectRatio() const {
    return static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

bool Window::hasWindowSizeChanged() const {
    return hasWindowChangedSize;
}

void Window::onWindowResize(GLFWwindow *window, const int width, const int height) {
    if (const auto windowHandle{static_cast<Window *>(glfwGetWindowUserPointer(window))}) {
        windowHandle->updateWindowSize(width, height);
    }
}

void Window::updateWindowSize(const int width, const int height) {
    windowWidth = width;
    windowHeight = height;

    // On some disploys (e.g., retina displays on macOS) the reported window resolution is a scaled resolution.
    // For example, a 3840x2160 screen may be used to display an up-scaled 1920x1080 screen.
    // GLFW will report 1920x1080 as the fullscreen resolution, but setting the OpenGL viewport to this resolution
    // results in viewport that only covers a quarter of the screen since OpenGL uses real pixels.
    // Using the framebuffer resolution ensures the viewport fills the window.
    int framebuffer_width, framebuffer_height;
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    glViewport(0, 0, framebuffer_width, framebuffer_height);
    hasWindowChangedSize = true;
}

void Window::onMouseScroll(GLFWwindow *window, double, const double scrollY) {
    if (const auto windowHandle{static_cast<Window *>(glfwGetWindowUserPointer(window))}) {
        windowHandle->scrollDelta += static_cast<float>(scrollY);
    }
}

void Window::updateMousePosition() {
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
