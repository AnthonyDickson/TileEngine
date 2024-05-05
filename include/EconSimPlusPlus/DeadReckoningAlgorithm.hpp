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
        /// The distance between two adjacent pixels in either the x or y direction.
        float distanceAdjacent;
        /// The diagonal distance bewteen two diagonally adjacent pixels.
        float distanceDiagonal;

    public:
        /// Create an instance of the dead reckoning algorithm.
        /// @param distanceAdjacent_ The distance between two adjacent pixels in either the x or y direction.
        /// @param distanceDiagonal_ The diagonal distance bewteen two diagonally adjacent pixels.
        explicit DeadReckoningAlgorithm(float distanceAdjacent_ = 1.0f, float distanceDiagonal_ = sqrt(2.0f));

        /// Generate a signed distance field from a binary image.
        /// @param buffer The single-channel image buffer containing a binary (black and white) image.
        /// @param resolution The width and height in pixels of the incoming image buffer.
        /// @return A single-channel image of the same resolution as the input.
        std::vector<float> createSDF(const std::uint8_t* buffer, glm::ivec2 resolution) const;

        /// Normalize an SDF and convert it to an 8-bit image.
        /// @param sdf A signed distance field.
        /// @param spread A factor that controls the range which is used to map the signed distance into the range of 0
        /// to 1 for storage in an 8-bit texture channel.
        /// @return A single channel image.
        static std::vector<std::uint8_t> createImage(const std::vector<float>& sdf, float spread = 8.0f);
    };

} // namespace EconSimPlusPlus

#endif // DEADRECKONINGALGORITHM_HPP
