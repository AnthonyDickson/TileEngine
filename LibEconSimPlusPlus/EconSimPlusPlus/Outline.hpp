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
// Created by Anthony Dickson on 05/06/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_OUTLINE_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_OUTLINE_HPP


#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Shader.hpp>


namespace EconSimPlusPlus::Outline {
    /// Where to draw the outline.
    enum class Placement { inset, outset };

    /// The configuration for the appearance of an outline.
    struct Style {
        /// The color of the outline.
        glm::vec4 color;
        /// The thickness of the outline in pixels.
        float thickness;
        /// Where to draw the outline.
        Placement placement;
    };

    /// Draw an outline around a GUI object.
    /// @note Assumes the shader has the uniform variables "transform" (glm::mat4) and "color" (glm::vec3).
    /// @note Assumes that the perspective/view matrices have already been set in the shader.
    /// @note Does not draw anything if the outline thickness is less than one pixel.
    /// @param object The GUI object to draw the outline around.
    /// @param shader The shader to draw the outline with.
    /// @param quad The quad geometry to use for drawing the outline.
    /// @param outline The configuration for the outline appearance.
    void draw(const Object& object, const Shader& shader, const Quad& quad, const Style& outline);

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_OUTLINE_HPP
