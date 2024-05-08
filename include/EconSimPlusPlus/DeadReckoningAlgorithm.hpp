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

#ifndef DEADRECKONINGALGORITHM_HPP
#define DEADRECKONINGALGORITHM_HPP

#include <glm/vec2.hpp>

#include <vector>

namespace EconSimPlusPlus {

    /// An algorithm for creating a signed distance field from a binary image.
    /// Adapted from the paper:
    /// Grevera, George J. "The “dead reckoning” signed distance transform."
    /// Computer Vision and Image Understanding 95, no. 3 (2004): 317-333.
    class DeadReckoningAlgorithm {
    public:
        /// Create a signed distance field (SDF) from a binary image.
        /// @param bitmap A black and white image where white pixels denote regions inside an object and black
        /// pixels regions outside an object.
        /// @param bitmapSize The width and height of the bitmap in pixels.
        /// @param paddedSize The desired width and height of the padded bitmap used for creating the SDF.
        /// @param outputSize The width and height in pixels of the output SDF image.
        /// @param spread A factor to divide the distance by. Larger values allows a larger range of values to be
        /// captured without being clipped.
        /// @return An 8-bit signed distance field (128.0f = 0).
        static std::vector<std::uint8_t> createSDF(const std::uint8_t* bitmap, glm::ivec2 bitmapSize,
                                                   glm::ivec2 paddedSize, glm::ivec2 outputSize, float spread = 16.0f);

        /// Pad an image with zeros.
        /// @param binaryImage An image buffer.
        /// @param inputSize The width and height of the image buffer in pixels.
        /// @param outputSize The desired width and height of the padded image buffer.
        /// @return The image buffer padded with zeros.
        template <typename PixelType>
        static std::vector<PixelType> padImage(const PixelType* binaryImage, glm::ivec2 inputSize,
                                               glm::ivec2 outputSize);

        /// Generate a signed distance field from a binary image.
        /// @param buffer The single-channel image buffer containing a binary (black and white) image.
        /// @param bufferSize The width and height in pixels of the input image buffer.
        /// @param distanceAdjacent The distance between two adjacent pixels in either the x or y direction.
        /// @param distanceDiagonal The diagonal distance bewteen two diagonally adjacent pixels.
        /// @return A single-channel image of the same resolution as the input.
        static std::vector<float> sdf(const std::uint8_t* buffer, glm::ivec2 bufferSize, float distanceAdjacent = 1.0f,
                                      float distanceDiagonal = sqrt(2.0f));

        /// Normalize an SDF and convert it to an 8-bit image.
        /// @param sdf A signed distance field.
        /// @param spread A factor that controls the range which is used to map the signed distance into the range of 0
        /// to 1 for storage in an 8-bit texture channel.
        /// @return A single channel image.
        static std::vector<std::uint8_t> createImage(const std::vector<float>& sdf, float spread = 8.0f);
    };

} // namespace EconSimPlusPlus

#endif // DEADRECKONINGALGORITHM_HPP
