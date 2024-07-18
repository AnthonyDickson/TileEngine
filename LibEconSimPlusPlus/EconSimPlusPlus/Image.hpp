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
// Created by Anthony Dickson on 18/07/2024.
//

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include "glm/vec2.hpp"

namespace EconSimPlusPlus::Image {
    struct Image {
        /// The raw byte data associated with the image.
        const std::vector<uint8_t> bytes;
        /// The size (width, height) of the texture in pixels.
        const glm::ivec2 resolution;
        /// The number of channels in the image data.
        ///
        /// Common channel counts:
        /// 1 = black and white,
        /// 3 = RGB,
        /// 4 = RGBA.
        const int channels;
        /// Where the texture was loaded from.
        const std::string path;
    };

    /// Load an image from a file path.
    /// @param imagePath The path to an image.
    Image create(const std::string& imagePath);
} // namespace EconSimPlusPlus

#endif // IMAGE_HPP
