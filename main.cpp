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

void framebuffer_size_callback(GLFWwindow *, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD. " << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set up vertices.
    float vertices[]{
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
    };

    unsigned int vboId{};
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Setup vertex shader.
    const char *vertexShaderSource{
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main() {\n"
            "    gl_Position = vec4(aPos, 1.0);\n"
            "}\0"
    };
    unsigned int vertexShaderID{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertexShaderID, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShaderID);

    int success{};
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Setup fragment shader.
    const char *fragmentShaderSource{
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0"
    };
    unsigned int fragmentShaderID{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShaderID);

    int fragSuccess{};
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragSuccess);

    if (!fragSuccess) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShaderID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create shader program and link shaders.
    unsigned shaderProgramID{glCreateProgram()};
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    int linkSuccess{};
    glGetShaderiv(shaderProgramID, GL_LINK_STATUS, &linkSuccess);

    if (!linkSuccess) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(shaderProgramID);

    // Create the vertex array object (VAO) and load the vertices.
    unsigned int vaoID{};
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgramID);
        glBindVertexArray(vaoID);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}