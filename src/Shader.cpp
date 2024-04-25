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

#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <EconSimPlusPlus/Shader.h>

Shader::Shader(const std::string& vertexShaderSourcePath, const std::string& fragmentShaderSourcePath) {
    // Load the shader source code.
    std::ifstream vertexShaderFile{};
    std::ifstream fragmentShaderFile{};

    std::string vertexShaderString{};
    std::string fragmentShaderString{};

    // ensure stream objects can throw exceptions:
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
    }
    catch (std::ifstream::failure&) {
        std::cout << "ERROR::SHADER::CANNOT_READ_FILE: " << vertexShaderSourcePath << ", ";
        std::cout << fragmentShaderSourcePath << std::endl;
    }

    const char* vertexShaderSource{vertexShaderString.c_str()};
    const char* fragmentShaderSource{fragmentShaderString.c_str()};

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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED " << vertexShaderSourcePath << "\n"
                  << infoLog << std::endl;
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
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " << fragmentShaderSourcePath << "\\n"
                  << infoLog << std::endl;
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

Shader::~Shader() {
    glDeleteShader(shaderProgramID);
}

void Shader::bind() const {
    glUseProgram(shaderProgramID);
}

int Shader::getUniformLocation(const std::string& name) const {
    const int location{glGetUniformLocation(shaderProgramID, name.c_str())};
    assert(location != -1 && "Uniform name does not exist in the shader source code.");

    return location;
}

[[maybe_unused]] void Shader::setUniform(const std::string& name, const bool value) const {
    setUniform(name, static_cast<int>(value));
}

void Shader::setUniform(const std::string& name, const int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, const float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(getUniformLocation(name), 1, value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat4x4& value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, value_ptr(value));
}

void Shader::setUniform(const std::string& name, const Material& value) const {
    setUniform(name + ".diffuse", value.diffuseTextureUnit);
    setUniform(name + ".specular", value.specularTextureUnit);
    setUniform(name + ".shininess", value.shininess);
}

void Shader::setUniform(const std::string& name, const Light& value) const {
    setUniform(name + ".position", value.position);
    setUniform(name + ".ambient", value.ambient);
    setUniform(name + ".diffuse", value.diffuse);
    setUniform(name + ".specular", value.specular);
}

void Shader::setUniform(const std::string& name, const DirectionalLight& value) const {
    setUniform(name + ".direction", value.direction);
    setUniform(name + ".color", value.color);
    setUniform(name + ".intensity", value.intensity);
}

void Shader::setUniform(const std::string& name, const SpotLight& value) const {
    setUniform(name + ".position", value.position);
    setUniform(name + ".direction", value.direction);

    setUniform(name + ".cutOff", value.cutOff);
    setUniform(name + ".outerCutOff", value.outerCutOff);

    setUniform(name + ".color", value.color);

    setUniform(name + ".linear", value.linear);
    setUniform(name + ".quadratic", value.quadratic);
}


void Shader::setUniform(const std::string& name, const PointLight& value) const {
    setUniform(name + ".position", value.position);

    setUniform(name + ".color", value.color);

    setUniform(name + ".linear", value.linear);
    setUniform(name + ".quadratic", value.quadratic);
}

void Shader::setUniform(const std::string& name, const std::vector<PointLight>& value) const {
    for (int i = 0; i < value.size(); ++i) {
        setUniform(std::format("{:s}[{:d}]", name, i), value[i]);
    }
}
