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
#include <unordered_map>
#include <vector>
#include <format>

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


void insertStructIdentifiers(std::unordered_set<std::string> &set, const std::string &identifier,
                             const std::vector<std::string> &memberNames) {
    for (const auto &memberName: memberNames) {
        set.insert(std::format("{}.{}", identifier, memberName));
    }
}

void
insertUniformName(std::unordered_set<std::string> &uniforms, const std::string &identifier, const std::string &type,
                  const std::unordered_map<std::string, std::vector<std::string>> &structTypes) {
    if (structTypes.contains(type)) {
        insertStructIdentifiers(uniforms, identifier, structTypes.at(type));
    } else {
        uniforms.insert(identifier);
    }
}

struct IndexedIdentifier {
    std::string identifier;
    int arraySize;
};

IndexedIdentifier
getIndexedIdentifier(const std::string &identifier, const std::unordered_map<std::string, int> &preprocessorVariables) {
    const auto firstBracketPosition = identifier.find_first_of('[');
    const auto arraySizeStringStart = firstBracketPosition + 1;
    const auto arraySizeStringEnd = identifier.find_last_not_of(']');
    const auto range = arraySizeStringEnd - arraySizeStringStart + 1;
    const auto arraySizeString = identifier.substr(arraySizeStringStart, range);

    int arraySize;

    if (preprocessorVariables.contains(arraySizeString)) {
        arraySize = preprocessorVariables.at(arraySizeString);
    } else {
        arraySize = std::stoi(arraySizeString);
    }

    return {identifier.substr(0, firstBracketPosition), arraySize};
}

std::unordered_set<std::string> Shader::extractUniformNames(const std::string &shaderSource) {
    std::unordered_set<std::string> uniformNames{};

    bool isInStruct{false};
    std::string currentStructName{};
    std::unordered_map<std::string, std::vector<std::string>> structTypes{};
    std::unordered_map<std::string, int> preprocessorVariables{};

    std::istringstream shaderSourceStream{shaderSource};

    for (std::string line; std::getline(shaderSourceStream, line, '\n');) {
        const auto trimmedLine{trim(line)};

        if (trimmedLine.empty() or trimmedLine.starts_with("//")) {
            continue;
        }

        if (trimmedLine.starts_with("void main()")) {
            break;
        }

        if (trimmedLine.starts_with("#define")) {
            std::string directive{};
            std::string identifier{};
            std::string value{};

            std::istringstream lineStream{trimmedLine};
            lineStream >> directive;
            lineStream >> identifier;
            lineStream >> value;

            preprocessorVariables[identifier] = std::stoi(value);
        } else if (trimmedLine.starts_with("struct")) {
            isInStruct = true;

            std::istringstream lineStream{trimmedLine};
            std::string qualifier{};
            lineStream >> qualifier;
            // The first token after 'struct' must be the name of the struct.
            lineStream >> currentStructName;
            structTypes[currentStructName] = {};
        } else if (isInStruct and trimmedLine.ends_with("};")) {
            isInStruct = false;
            currentStructName.clear();
        } else if (isInStruct and !trimmedLine.starts_with('{')) {
            std::istringstream lineStream{trimmedLine};
            std::string type{};
            std::string identifier{};
            lineStream >> type;
            lineStream >> identifier;

            if (identifier.ends_with(';')) {
                identifier.pop_back();
            }

            structTypes[currentStructName].push_back(identifier);
        } else if (trimmedLine.starts_with("uniform")) {
            std::istringstream lineStream{trimmedLine};
            std::string qualifier{};
            std::string type{};
            std::string identifier{};

            lineStream >> qualifier;
            lineStream >> type;
            lineStream >> identifier;

            if (identifier.ends_with(';')) {
                identifier.pop_back();
            }

            int arraySize{0};

            if (identifier.ends_with(']')) {
                const auto indexedIdentifier = getIndexedIdentifier(identifier, preprocessorVariables);
                identifier = indexedIdentifier.identifier;
                arraySize = indexedIdentifier.arraySize;
            }

            if (arraySize > 0) {
                for (int i = 0; i < arraySize; ++i) {
                    std::string indexedIdentifier{std::format("{:s}[{:d}]", identifier, i)};

                    insertUniformName(uniformNames, indexedIdentifier, type, structTypes);
                }
            } else {
                insertUniformName(uniformNames, identifier, type, structTypes);
            }
        }
    }

    return uniformNames;
}

std::string Shader::trim(const std::string &string, const std::string &whitespaceChars) {
    const auto firstNonWhitespace{string.find_first_not_of(whitespaceChars)};
    const auto lastNonWhitespace{string.find_last_not_of(whitespaceChars)};

    if (firstNonWhitespace == std::string::npos or lastNonWhitespace == std::string::npos) {
        return std::string{};
    }

    return {string.substr(firstNonWhitespace, lastNonWhitespace - firstNonWhitespace + 1)};
}

Shader::~Shader() {
    glDeleteShader(shaderProgramID);
}

void Shader::use() const {
    glUseProgram(shaderProgramID);
}

int Shader::getUniformLocation(const std::string &name) const {
    assert(uniformNames.contains(name) && "Uniform name does not exist in the shader source code.");
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

void Shader::setUniform(const std::string &name, const DirectionalLight &value) const {
    setUniform(name + ".direction", value.direction);
    setUniform(name + ".color", value.color);
    setUniform(name + ".intensity", value.intensity);
}

void Shader::setUniform(const std::string &name, const SpotLight &value) const {
    setUniform(name + ".position", value.position);
    setUniform(name + ".direction", value.direction);

    setUniform(name + ".cutOff", value.cutOff);
    setUniform(name + ".outerCutOff", value.outerCutOff);

    setUniform(name + ".color", value.color);

    setUniform(name + ".linear", value.linear);
    setUniform(name + ".quadratic", value.quadratic);
}


void Shader::setUniform(const std::string &name, const PointLight &value) const {
    setUniform(name + ".position", value.position);

    setUniform(name + ".color", value.color);

    setUniform(name + ".linear", value.linear);
    setUniform(name + ".quadratic", value.quadratic);
}

void Shader::setUniform(const std::string &name, const std::vector<PointLight> &value) const {
    for (int i = 0; i < value.size(); ++i) {
        setUniform(std::format("{:s}[{:d}]", name, i), value[i]);
    }
}
