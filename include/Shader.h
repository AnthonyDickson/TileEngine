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

#include <string>
#include <vector>

#include "glm/detail/type_mat4x4.hpp"

#include "light/Light.h"
#include "light/DirectionalLight.h"
#include "light/SpotLight.h"
#include "light/PointLight.h"
#include "Material.h"

/**
 * Handles the loading, compilation, linking and usage of an OpenGL shader program.
 */
class Shader {
private:
    /** The ID of the shader program in OpenGL. */
    unsigned int shaderProgramID{};
public:
    /**
     * Load and compile GLSL shaders from disk.
     * @param vertexShaderSourcePath The path to the vertex shader source code.
     * @param fragmentShaderSourcePath The path to the fragment shader source code.
     */
    Shader(const std::string &vertexShaderSourcePath, const std::string &fragmentShaderSourcePath);

    /** Clean up OpenGL related stuff. */
    ~Shader();

    /**
     * Activate the shader program.
     */
    void use() const;

    /**
     * Get the location of a uniform variable in the shader program.
     * @param name The name of the uniform in the shader source code.
     * @return An integer indicating the location.
     */
    [[nodiscard]] int getUniformLocation(const std::string &name) const;

    /**
     * Set a bool uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    [[maybe_unused]] void setUniform(const std::string &name, bool value) const;

    /**
     * Set an integer uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setUniform(const std::string &name, int value) const;

    /**
     * Set a float uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    [[maybe_unused]] void setUniform(const std::string &name, float value) const;

    /**
     * Set a float 3-vector uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setUniform(const std::string &name, const glm::vec3 &value) const;

    /**
     * Set a 4x4 float matrix uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setUniform(const std::string &name, const glm::mat4x4 &value) const;

    /**
     * Set a material uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setUniform(const std::string &name, const Material &value) const;

    /**
     * Set a light uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    [[maybe_unused]] void setUniform(const std::string &name, const Light &value) const;

    /**
     * Set a directional light uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setUniform(const std::string &name, const DirectionalLight &value) const;

    /**
     * Set a spotlight uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setUniform(const std::string &name, const SpotLight &value) const;

    /**
     * Set a point light uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setUniform(const std::string &name, const PointLight &value) const;

    /**
     * Set point light uniform values.
     * @param name The name of the uniform.
     * @param value The values to set the uniform to.
     */
    void setUniform(const std::string &name, const std::vector<PointLight> &value) const;
};


#endif //ECONSIMPLUSPLUS_SHADER_H
