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
#include <iostream>

#include "glad/glad.h"

#include <EconSimPlusPlus/Window.hpp>

namespace EconSimPlusPlus {
    bool Window::m_isInitialised = false;

    Window::Window(const int windowWidth_, const int windowHeight_, const std::string& windowName) :
        m_windowWidth(windowWidth_), m_windowHeight(windowHeight_) {
        assert(!m_isInitialised && "Cannot have more than one instance of `Window`.");

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, windowName.c_str(), nullptr, nullptr);

        if (m_window == nullptr) {
            throw std::runtime_error("Failed to create the GLFW window.");
        }

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            throw std::runtime_error("Failed to initialize GLAD.");
        }

        updateWindowSize(windowWidth_, windowHeight_);
        glfwSetWindowSizeCallback(m_window, onWindowResize);
        glfwSetScrollCallback(m_window, onMouseScroll);
        glfwSetWindowUserPointer(m_window, this);
        glfwSwapInterval(0); // Let the game handle vsync

        m_isInitialised = true;
    }

    Window::~Window() {
        glfwTerminate();
    }

    void Window::preUpdate() {
        m_inputState.update(m_window);
    }

    void Window::postUpdate() {
        m_inputState.postUpdate();
        m_hasWindowChangedSize = false;

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(m_window);
    }

    const InputState& Window::inputState() const {
        return m_inputState;
    }

    int Window::width() const {
        return m_windowWidth;
    }

    int Window::height() const {
        return m_windowHeight;
    }

    glm::ivec2 Window::size() const {
        return {m_windowWidth, m_windowHeight};
    }

    bool Window::hasWindowSizeChanged() const {
        return m_hasWindowChangedSize;
    }

    void Window::setCursor(const int standardCursorType) {
        if (m_cursor != nullptr) {
            glfwDestroyCursor(m_cursor);
        }

        m_cursor = glfwCreateStandardCursor(standardCursorType);
        glfwSetCursor(m_window, m_cursor);
    }

    void Window::onWindowResize(GLFWwindow* window, const int width, const int height) {
        if (const auto windowHandle{static_cast<Window*>(glfwGetWindowUserPointer(window))}) {
            windowHandle->updateWindowSize(width, height);
        }
    }

    void Window::updateWindowSize(const int width, const int height) {
        m_windowWidth = width;
        m_windowHeight = height;

        // On some disploys (e.g., retina displays on macOS) the reported window resolution is a scaled resolution.
        // For example, a 3840x2160 screen may be used to display an up-scaled 1920x1080 screen.
        // GLFW will report 1920x1080 as the fullscreen resolution, but setting the OpenGL viewport to this resolution
        // results in viewport that only covers a quarter of the screen since OpenGL uses real pixels.
        // Using the framebuffer resolution ensures the viewport fills the window.
        int framebuffer_width, framebuffer_height;
        glfwGetFramebufferSize(m_window, &framebuffer_width, &framebuffer_height);
        glViewport(0, 0, framebuffer_width, framebuffer_height);
        m_hasWindowChangedSize = true;
    }

    void Window::onMouseScroll(GLFWwindow* window, const double scrollX, const double scrollY) {
        if (const auto windowHandle{static_cast<Window*>(glfwGetWindowUserPointer(window))}) {
            windowHandle->m_inputState.updateScroll(scrollX, scrollY);
        }
    }

    glm::vec2 topLeft(const Window& window) {
        return {-0.5f * static_cast<float>(window.width()), 0.5f * static_cast<float>(window.height())};
    }

    glm::vec2 topRight(const Window& window) {
        return 0.5f * static_cast<glm::vec2>(window.size());
    }
} // namespace EconSimPlusPlus
