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
// Created by Anthony Dickson on 07/07/2024.
//

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/Quad.hpp>

namespace EconSimPlusPlus {

    /// A wrapper for commonly used rendering objects.
    struct Graphics {
        /// The camera used for rendering objects on screen.
        Camera camera;
        /// The default font used for rendering text.
        std::unique_ptr<Font> font{Font::create("resource/font/Roboto-Regular.ttf", {288, 288}, {64, 64}, 32.0f)};
        /// The shader intended for drawing a quad with an RGBA color.
        Shader quadShader{Shader::create("resource/shader/grid.vert", "resource/shader/rgba.frag")};
        // TODO: Create wrapper around quad and solid fill shader so that user only has to pass in camera + transform?
        /// A unit quad (width == height == 1 px) positioned at the world origin.
        Quad quad{};
    };

} // namespace EconSimPlusPlus

#endif // GRAPHICS_HPP
