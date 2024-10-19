

//
// Created by Anthony on 23/03/2024.
//

#include <format>
#include <fstream>
#include <sstream>

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <TileEngine/Shader.hpp>

namespace TileEngine {
    Shader Shader::create(const std::string& vertexShaderSourcePath, const std::string& fragmentShaderSourcePath,
                          const int maxInstances) {
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
            throw std::ifstream::failure(std::format("ERROR::SHADER::CANNOT_READ_FILE: {:s}, {:s}",
                                                     vertexShaderSourcePath, fragmentShaderSourcePath));
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
            throw std::runtime_error(
                std::format("ERROR::SHADER::VERTEX::COMPILATION_FAILED {:s}\n{:s}\n", vertexShaderSourcePath, infoLog));
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

            throw std::runtime_error(
                std::format("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED {:s}\n{:s}\n", fragmentShaderSourcePath, infoLog));
        }

        // Compile the shader program.
        unsigned int shaderProgramID = glCreateProgram();
        glAttachShader(shaderProgramID, vertexShaderId);
        glAttachShader(shaderProgramID, fragmentShaderId);
        glLinkProgram(shaderProgramID);

        int linkingWasSuccessful{};
        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &linkingWasSuccessful);

        if (!linkingWasSuccessful) {
            char infoLog[infoLogSize];
            glGetProgramInfoLog(shaderProgramID, infoLogSize, nullptr, infoLog);
            throw std::runtime_error(std::format("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{:s}\n", infoLog));
        }

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);

        return {shaderProgramID, maxInstances};
    }

    Shader::Shader(const unsigned int shaderProgramID, const int maxInstances) :
        m_shaderProgramID(shaderProgramID), m_maxInstances(maxInstances) {
    }

    Shader::~Shader() {
        glDeleteShader(m_shaderProgramID);
    }

    int Shader::maxInstances() const {
        return m_maxInstances;
    }

    void Shader::bind() const {
        glUseProgram(m_shaderProgramID);
    }

    int Shader::uniformLocation(const std::string& name) const {
        const int location{glGetUniformLocation(m_shaderProgramID, name.c_str())};
        assert(location != -1 && "Uniform name does not exist in the shader source code.");

        return location;
    }

    void Shader::setUniform(const std::string& name, const bool value) const {
        setUniform(name, static_cast<int>(value));
    }

    void Shader::setUniform(const std::string& name, const int value) const {
        glUniform1i(uniformLocation(name), value);
    }

    void Shader::setUniform(const std::string& name, const float value) const {
        glUniform1f(uniformLocation(name), value);
    }

    void Shader::setUniform(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(uniformLocation(name), 1, value_ptr(value));
    }

    void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(uniformLocation(name), 1, value_ptr(value));
    }

    void Shader::setUniform(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(uniformLocation(name), 1, value_ptr(value));
    }

    void Shader::setUniform(const std::string& name, const glm::mat4x4& value) const {
        glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, value_ptr(value));
    }
} // namespace TileEngine
