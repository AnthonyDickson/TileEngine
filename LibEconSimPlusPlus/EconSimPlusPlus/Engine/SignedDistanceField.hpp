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
// Created by Anthony Dickson on 04/05/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_SIGNEDDISTANCEFIELD_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_SIGNEDDISTANCEFIELD_HPP

#include <vector>

#include <glm/vec2.hpp>

namespace EconSimPlusPlus::Engine::SignedDistanceField {
    /// Create a signed distance field (SDF) from a binary image.
    /// @param bitmap A black and white image where white pixels denote regions inside an object and black
    /// pixels regions outside an object.
    /// @param bitmapSize The width and height of the bitmap in pixels.
    /// @param paddedSize The desired width and height of the padded bitmap used for creating the SDF.
    /// @param outputSize The width and height in pixels of the output SDF image.
    /// @param spread A factor to divide the distance by. Larger values allows a larger range of values to be
    /// captured without being clipped.
    /// @return An 8-bit signed distance field (128.0f = 0).
    std::vector<std::uint8_t> createSDF(const std::uint8_t* bitmap, glm::ivec2 bitmapSize, glm::ivec2 paddedSize,
                                        glm::ivec2 outputSize, float spread = 16.0f);
} // namespace EconSimPlusPlus::Engine::SignedDistanceField

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_SIGNEDDISTANCEFIELD_HPP
