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
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.ses/>.

//
// Created by Anthony on 23/03/2024.
//

#ifndef ECONSIMPLUSPLUS_SHADER_H
#define ECONSIMPLUSPLUS_SHADER_H

#include <string>

/**
 * Handles the loading, compilation, linking and usage of an OpenGL shader program.
 */
class Shader {
public:
    /** The ID of the shader program in OpenGL. */
    unsigned int shaderProgramID;

    /**
     * Load and compile GLSL shaders from disk.
     * @param vertexShaderSourcePath The path to the vertex shader source code.
     * @param fragmentShaderSourcePath The path to the fragment shader source code.
     */
    Shader(const std::string &vertexShaderSourcePath, const std::string &fragmentShaderSourcePath);

    /**
     * Activate the shader program.
     */
    void use() const;

    /** Clean up OpenGL related stuff. */
    void cleanup() const;

    /**
     * Set a bool uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setBool(const std::string &name, bool value) const;

    /**
     * Set an integer uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setInt(const std::string &name, int value) const;

    /**
     * Set a float uniform value.
     * @param name The name of the uniform.
     * @param value The value to set the uniform to.
     */
    void setFloat(const std::string &name, float value) const;
};


#endif //ECONSIMPLUSPLUS_SHADER_H
