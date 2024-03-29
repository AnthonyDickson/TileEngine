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
// Created by Anthony on 31/10/2023.
//
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include "Camera.h"

namespace constants {
    [[maybe_unused]] constexpr float cube[]{
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,

            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, 0.5f,
            0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, 0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, -0.5f,
    };

    [[maybe_unused]] constexpr float texturedCube[]{
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };
}

void framebuffer_size_callback(GLFWwindow *, int width, int height) {
    glViewport(0, 0, width, height);
}

void handleKeyboardInput(GLFWwindow *window, Camera &camera, float cameraSpeed) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move(Camera::Direction::forward, cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move(Camera::Direction::backward, cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(Camera::Direction::left, cameraSpeed);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(Camera::Direction::right, cameraSpeed);
    }
}

void handleMouseInput(GLFWwindow *window, Camera &camera) {
    double x{};
    double y{};
    glfwGetCursorPos(window, &x, &y);

    const auto mousePosition = glm::vec2{static_cast<float>(x), static_cast<float>(y)};
    camera.updateRotation(mousePosition);
}

void handleMouseScroll(GLFWwindow *window, double scrollX, double scrollY) {
    // This function is a workaround to use non-static functions as callbacks with glfw.
    Camera *camera{reinterpret_cast<Camera *>(glfwGetWindowUserPointer(window))};

    if (camera) {
        camera->handleMouseScroll(scrollX, scrollY);
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    constexpr int windowWidth{800};
    constexpr int windowHeight{600};

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, handleMouseScroll);

    // Create the vertex array object.
    unsigned int vaoID{};
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // Set up and buffer vertices.
    unsigned int vboID{};
    glGenBuffers(1, &vboID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(constants::cube), constants::cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Setup light cube while reusing the buffered data from the previous cube.
    unsigned int lightVaoID{};
    glGenVertexArrays(1, &lightVaoID);
    glBindVertexArray(lightVaoID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    // No need to buffer vertex data here since we are reusing the vertex data from the previous cube.

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


    Shader shader{"resource/shader/color.vert", "resource/shader/color.frag"};

    glm::vec3 cubePositions[]{
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    glm::vec3 lightPosition{1.2f, 1.0f, -2.0f};
    glm::mat4 lightModelMatrix{1.0f};
    lightModelMatrix = glm::translate(lightModelMatrix, lightPosition);
    lightModelMatrix = glm::scale(lightModelMatrix, glm::vec3{0.2f});

    constexpr float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    Camera camera{aspectRatio, glm::vec3(0.0f, 0.0f, 3.0f)};
    glfwSetWindowUserPointer(window, reinterpret_cast<void *>(&camera));

    auto lastFrameTime = static_cast<float>(glfwGetTime());
    constexpr const float cameraMoveSpeed = 2.5f;

    glm::vec3 objectColor{1.0f, 0.5f, 0.31f};
    glm::vec3 lightColor{1.0f, 1.0f, 1.0f};

    while (!glfwWindowShouldClose(window)) {
        const auto currentFrameTime = static_cast<float>(glfwGetTime());
        const auto deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        handleKeyboardInput(window, camera, deltaTime * cameraMoveSpeed);
        handleMouseInput(window, camera);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        shader.use();
        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getPerspectiveMatrix());
        shader.setVec3("objectColor", objectColor);
        shader.setVec3("lightColor", lightColor);

        glBindVertexArray(vaoID);

        for (int i = 0; i < 10; i++) {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(20.0f * static_cast<float>(i)),
                                glm::vec3(1.0f, 0.3f, 0.5f));

            shader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Draw the light source.
        shader.setVec3("objectColor", lightColor);
        shader.setMat4("model", lightModelMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &vaoID);
    glDeleteBuffers(1, &vboID);
    shader.cleanup();

    glfwTerminate();
    return 0;
}