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

#ifndef ECONSIMPLUSPLUS_SHADER_H
#define ECONSIMPLUSPLUS_SHADER_H

#include "glm/mat4x4.hpp"

namespace EconSimPlusPlus {
    /// Handles the loading, compilation, linking and usage of an OpenGL shader program.
    class Shader {
        /// The ID of the shader program in OpenGL.
        unsigned int shaderProgramID{};

    public:
        /// The max number of instances supported by the shader.
        const int maxInstances;

        /// Load and compile GLSL shaders from disk.
        /// @param vertexShaderSourcePath The path to the vertex shader source code.
        /// @param fragmentShaderSourcePath The path to the fragment shader source code.
        /// @param maxInstances_ The max number of instances that can be used at once.
        Shader(const std::string& vertexShaderSourcePath, const std::string& fragmentShaderSourcePath,
               int maxInstances_ = 128);

        /// Delete copy constructor to avoid OpenGL issues.
        Shader(Shader&) = delete;
        /// Delete move constructor to avoid OpenGL issues.
        Shader(Shader&&) = delete;

        /// Clean up OpenGL related stuff.
        ~Shader();

        /// Activate the shader program.
        void bind() const;

        /// Get the location of a uniform variable in the shader program.
        /// @param name The name of the uniform in the shader source code.
        /// @return An integer indicating the location.
        [[nodiscard]] int getUniformLocation(const std::string& name) const;

        /// Set a bool uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        [[maybe_unused]] void setUniform(const std::string& name, bool value) const;

        /// Set an integer uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        void setUniform(const std::string& name, int value) const;

        /// Set a float uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        [[maybe_unused]] void setUniform(const std::string& name, float value) const;

        /// Set a float 3-vector uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        void setUniform(const std::string& name, const glm::vec3& value) const;

        /// Set a float 2-vector uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        void setUniform(const std::string& name, const glm::vec2& value) const;

        /// Set a 4x4 float matrix uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        void setUniform(const std::string& name, const glm::mat4x4& value) const;
    };
} // namespace EconSimPlusPlus

#endif // ECONSIMPLUSPLUS_SHADER_H
