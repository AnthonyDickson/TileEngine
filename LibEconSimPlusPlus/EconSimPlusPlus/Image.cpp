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
#include <format>

#include "stb_image.h"

#include <EconSimPlusPlus/Image.hpp>

namespace EconSimPlusPlus::Image {
    Image create(const std::string& imagePath) {
        int width{};
        int height{};
        int channelCount{};
        stbi_set_flip_vertically_on_load(true);
        const auto byteBuffer{stbi_load(imagePath.c_str(), &width, &height, &channelCount, 0)};

        if (byteBuffer == nullptr) {
            throw std::runtime_error(
                std::format("Texture failed to load image from {0}: {1}", imagePath, stbi_failure_reason()));
        }

        const std::vector imageData(byteBuffer, byteBuffer + width * height * channelCount);

        stbi_image_free(byteBuffer);

        return {imageData, glm::ivec2{width, height}, channelCount, imagePath};
    }
} // namespace EconSimPlusPlus
