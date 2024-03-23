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
// Created by Anthony on 23/03/2024.
//

#include <fstream>
#include <iostream>
#include <sstream>
#include "shader.h"
#include "glad/glad.h"

Shader::Shader(const std::string &vertexShaderSourcePath, const std::string &fragmentShaderSourcePath) {
    // Load the shader source code.
    std::ifstream vertexShaderFile{};
    std::ifstream fragmentShaderFile{};

    std::string vertexShaderString{};
    std::string fragmentShaderString{};

    // ensure ifstream objects can throw exceptions:
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexShaderFile.open(vertexShaderSourcePath);
        fragmentShaderFile.open(fragmentShaderSourcePath);

        std::stringstream vertexShaderStream{};
        std::stringstream fragmentShaderStream{};

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderString = vertexShaderStream.str();
        fragmentShaderString = fragmentShaderStream.str();

        vertexShaderFile.close();
        fragmentShaderFile.close();
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::CANNOT_READ_FILE: " << vertexShaderSourcePath << ", ";
        std::cout << fragmentShaderSourcePath << std::endl;
    }

    const char *vertexShaderSource{vertexShaderString.c_str()};
    const char *fragmentShaderSource{fragmentShaderString.c_str()};

    // Compile the vertex shader.
    unsigned int vertexShaderId{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShaderId);

    int vertexShaderCompiled{};
    glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &vertexShaderCompiled);

    constexpr int infoLogSize{512};

    if (!vertexShaderCompiled) {
        char infoLog[infoLogSize];
        glGetShaderInfoLog(vertexShaderId, infoLogSize, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile the fragment shader.
    unsigned int fragmentShaderId{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShaderId);

    int fragmentShaderCompiled{};
    glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &fragmentShaderCompiled);

    if (!fragmentShaderCompiled) {
        char infoLog[infoLogSize];
        glGetShaderInfoLog(fragmentShaderId, infoLogSize, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile the shader program.
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderId);
    glAttachShader(shaderProgramID, fragmentShaderId);
    glLinkProgram(shaderProgramID);

    int linkingWasSuccessful{};
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &linkingWasSuccessful);

    if (!linkingWasSuccessful) {
        char infoLog[infoLogSize];
        glGetProgramInfoLog(shaderProgramID, infoLogSize, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);
}

void Shader::use() const {
    glUseProgram(shaderProgramID);
}

void Shader::cleanup() const {
    glDeleteShader(shaderProgramID);
}

void Shader::setBool(const std::string &name, bool value) const {
    int uniformLocation = glGetUniformLocation(shaderProgramID, name.c_str());
    glUniform1i(uniformLocation, value);
}

void Shader::setInt(const std::string &name, int value) const {
    int uniformLocation = glGetUniformLocation(shaderProgramID, name.c_str());
    glUniform1i(uniformLocation, value);
}

void Shader::setFloat(const std::string &name, float value) const {
    int uniformLocation = glGetUniformLocation(shaderProgramID, name.c_str());
    glUniform1f(uniformLocation, value);
}
