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
#include <algorithm>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

Shader::Shader(const std::string &vertexShaderSourcePath, const std::string &fragmentShaderSourcePath) {
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
    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::CANNOT_READ_FILE: " << vertexShaderSourcePath << ", ";
        std::cout << fragmentShaderSourcePath << std::endl;
    }

    const char *vertexShaderSource{vertexShaderString.c_str()};
    const char *fragmentShaderSource{fragmentShaderString.c_str()};

    uniformNames = extractUniformNames(vertexShaderString);
    uniformNames.merge(extractUniformNames(fragmentShaderString));

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
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED " << vertexShaderSourcePath << "\n" << infoLog
                  << std::endl;
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
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " << fragmentShaderSourcePath << "\\n" << infoLog
                  << std::endl;
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

std::unordered_set<std::string> Shader::extractUniformNames(const std::string &shaderSource) {
    std::unordered_set<std::string> uniformNames{};
    std::istringstream shaderSourceStream{shaderSource};
    // TODO: Include struct uniforms.
    // TODO: Parse structs and store member names.
    // TODO: If uniform is a struct, add the identifier and the struct member identifiers joined by a period.

    for (std::string line; std::getline(shaderSourceStream, line, '\n');) {
        if (line.empty() or !line.starts_with("uniform")) {
            continue;
        }

        if (line.starts_with("void main()")) {
            break;
        }

        const auto lastWhitespace{line.rfind(' ')};

        if (lastWhitespace == std::string::npos) {
            continue;
        }

        std::string identifier{line.substr(lastWhitespace + 1)};

        if (identifier.ends_with(';')) {
            identifier.pop_back();
        }

        uniformNames.insert(identifier);
    }

    return uniformNames;
}

Shader::~Shader() {
    glDeleteShader(shaderProgramID);
}

void Shader::use() const {
    glUseProgram(shaderProgramID);
}

int Shader::getUniformLocation(const std::string &name) const {
#ifdef SHADER_VERIFY_UNIFORM_NAMES
    assert(uniformNames.contains(name) && "Uniform name does not exist in the shader source code.");
#endif
    return glGetUniformLocation(shaderProgramID, name.c_str());
}

[[maybe_unused]] void Shader::setUniform(const std::string &name, bool value) const {
    setUniform(name, static_cast<int>(value));
}

void Shader::setUniform(const std::string &name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string &name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string &name, const glm::mat4x4 &value) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string &name, const Material &value) const {
    setUniform(name + ".diffuse", value.diffuseTextureUnit);
    setUniform(name + ".specular", value.specularTextureUnit);
    setUniform(name + ".shininess", value.shininess);
}

void Shader::setUniform(const std::string &name, const Light &value) const {
    setUniform(name + ".position", value.position);
    setUniform(name + ".ambient", value.ambient);
    setUniform(name + ".diffuse", value.diffuse);
    setUniform(name + ".specular", value.specular);
}
