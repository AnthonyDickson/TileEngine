

//
// Created by Anthony on 23/03/2024.
//

#ifndef LIBTILEENGINE_TILEENGINE_SHADER_HPP
#define LIBTILEENGINE_TILEENGINE_SHADER_HPP

// ReSharper disable once CppUnusedIncludeDirective
#include <string>

#include "glm/mat4x4.hpp"

namespace TileEngine {
    /// Handles the loading, compilation, linking and usage of an OpenGL shader program.
    class Shader {
    public:
        /// Load, compile and link GLSL shaders from disk.
        /// @param vertexShaderSourcePath The path to the vertex shader source code.
        /// @param fragmentShaderSourcePath The path to the fragment shader source code.
        /// @param maxInstances The max number of instances that can be used at once.
        /// @return A Shader object for using the compiled shader program.
        static Shader create(const std::string& vertexShaderSourcePath, const std::string& fragmentShaderSourcePath,
                             int maxInstances = 128);

        /// Create a Shader object.
        /// @param shaderProgramID The ID of the shader program in OpenGL.
        /// @param maxInstances The max number of instances supported by the shader.
        Shader(unsigned int shaderProgramID, int maxInstances);

        /// Delete copy constructor to avoid OpenGL issues.
        Shader(Shader&) = delete;
        /// Delete move constructor to avoid OpenGL issues.
        Shader(Shader&&) = delete;

        /// Clean up OpenGL related stuff.
        ~Shader();

        /// Get the max number of instances supported by the shader.
        [[nodiscard]] int maxInstances() const;

        /// Activate the shader program.
        void bind() const;

        /// Get the location of a uniform variable in the shader program.
        /// @param name The name of the uniform in the shader source code.
        /// @return An integer indicating the location.
        [[nodiscard]] int uniformLocation(const std::string& name) const;

        /// Set a bool uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        void setUniform(const std::string& name, bool value) const;

        /// Set an integer uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        void setUniform(const std::string& name, int value) const;

        /// Set a float uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        void setUniform(const std::string& name, float value) const;

        /// Set a float 4-vector uniform value.
        /// @param name The name of the uniform.
        /// @param value The value to set the uniform to.
        void setUniform(const std::string& name, const glm::vec4& value) const;

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

    private:
        /// The ID of the shader program in OpenGL.
        const unsigned int m_shaderProgramID{};
        /// The max number of instances supported by the shader.
        const int m_maxInstances;
    };
} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_SHADER_HPP
