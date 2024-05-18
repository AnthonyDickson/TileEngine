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
// Created by Anthony on 24/03/2024.
//

#include <format>
#include <iostream>

#include <glm/vec2.hpp>
#include "stb_image.h"

#include <EconSimPlusPlus/Engine/Texture.hpp>

namespace EconSimPlusPlus {

    std::unique_ptr<Texture> Texture::create(const std::string& imagePath, const int textureUnit_) {
        int width{};
        int height{};
        int channelCount{};
        stbi_set_flip_vertically_on_load(true);
        const auto imageData{stbi_load(imagePath.c_str(), &width, &height, &channelCount, 0)};

        if (imageData == nullptr) {
            throw std::runtime_error(
                std::format("Texture failed to load image from {0}: {1}", imagePath, stbi_failure_reason()));
        }

        std::unique_ptr texture{create(imageData, {width, height}, channelCount, textureUnit_)};

        stbi_image_free(imageData);

        return texture;
    }

    std::unique_ptr<Texture> Texture::create(const unsigned char* image, const glm::ivec2 resolution,
                                             const int channelCount, const int textureUnit_) {
        GLenum imageFormat;

        switch (channelCount) {
        case 1:
            imageFormat = GL_RED;
            break;
        case 3:
            imageFormat = GL_RGB;
            break;
        case 4:
            imageFormat = GL_RGBA;
            break;
        default:
            throw std::runtime_error(std::format("Texture does not support image with {0} channels.", channelCount));
        }

        GLuint textureID{};
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(imageFormat), resolution.x, resolution.y, 0, imageFormat,
                     GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return std::make_unique<Texture>(textureID, textureUnit_, resolution);
    }

    Texture::Texture(const unsigned int textureID_, const int textureUnit_, const glm::ivec2 resolution_) :
        m_textureID(textureID_), m_textureUnit(textureUnit_), m_resolution(resolution_) {
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_textureID);
    }

    glm::ivec2 Texture::resolution() const {
        return m_resolution;
    }

    void Texture::bind() const {
        glActiveTexture(m_textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    int Texture::getUniformTextureUnit() const {
        return m_textureUnit - GL_TEXTURE0;
    }
} // namespace EconSimPlusPlus
