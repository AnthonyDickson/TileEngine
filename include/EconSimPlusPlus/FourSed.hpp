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
// Created by Anthony Dickson on 07/05/2024.
//

#ifndef FOURSED_HPP
#define FOURSED_HPP

#include <vector>

#include "glm/vec2.hpp"

namespace EconSimPlusPlus {

    /// Implements the 4SED (Four-point Sequential Euclidean Distance) mapping algorithm from:
    /// Danielsson, Per-Erik. "Euclidean distance mapping."
    /// Computer Graphics and image processing 14, no. 3 (1980): 227-248.
    class FourSED {
    public:
        /// Pad an image with zeros.
        /// @param binaryImage An image buffer.
        /// @param inputSize The width and height of the image buffer in pixels.
        /// @param outputSize The desired width and height of the padded image buffer.
        /// @return The image buffer padded with zeros.
        static std::vector<std::uint8_t> padImage(const std::uint8_t* binaryImage, glm::ivec2 inputSize,
                                                  glm::ivec2 outputSize);

        /// Perform a Euclidean distance transform on a binary image.
        /// In the output, each pixel denotes the distance from the nearest object.
        /// @param binaryImage A black and white image where white pixels denote regions inside an object and black
        /// pixels regions outside an object.
        /// @param inputSize The width and height of the input image in pixels.
        /// @param flipBits If `true`, treat 'inside' pixels as 'outside', i.e. white pixels as black and vice versa.
        /// @return A floating point distance field the same size as the input image.
        static std::vector<float> edt(const std::uint8_t* binaryImage, glm::ivec2 inputSize, bool flipBits = false);

        /// Convert a floating point distance field to an 8-bit image.
        /// @param distanceField A floating point image where each pixel denotes the distance to the nearest object.
        /// @return An 8-bit distance field.
        static std::vector<std::uint8_t> createImage(const std::vector<float>& distanceField);

        /// Combine two distance fields into a signed distance field.
        /// @param insideDistanceField The distance field indicating the distance from within an object to the nearest
        /// object border pixel.
        /// @param outsideDistanceField The distance field indicating the distance from outside an object to the nearest
        /// object border pixel.
        /// @param spread A factor to divide the distance by. Larger values allows a larger range of values to be
        /// captured without being clipped.
        /// @return An 8-bit distance field.
        static std::vector<std::uint8_t> createImage(const std::vector<float>& insideDistanceField,
                                                     const std::vector<float>& outsideDistanceField,
                                                     float spread = 8.0f);
    };

} // namespace EconSimPlusPlus

#endif // FOURSED_HPP
