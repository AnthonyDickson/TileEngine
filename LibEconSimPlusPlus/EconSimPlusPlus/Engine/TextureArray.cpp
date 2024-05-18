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

#include "glad/glad.h"

#include <EconSimPlusPlus/Engine/TextureArray.hpp>

namespace EconSimPlusPlus::Engine {
    TextureArray::TextureArray(const unsigned int id_) : m_id(id_) {
    }
    TextureArray::~TextureArray() {
        glDeleteTextures(1, &m_id);
    }
    std::unique_ptr<TextureArray> TextureArray::create(const int depth_, const glm::ivec2 resolution_) {
        unsigned int textureArrayID;
        glGenTextures(1, &textureArrayID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, resolution_.x, resolution_.y, depth_, 0, GL_RED, GL_UNSIGNED_BYTE,
                     nullptr);

        return std::make_unique<TextureArray>(textureArrayID);
    }
    void TextureArray::bufferSubImage(const int zOffset, const glm::ivec2 bufferSize,
                                      const unsigned char* buffer) const {
        bind();
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, zOffset, bufferSize.x, bufferSize.y, 1, GL_RED, GL_UNSIGNED_BYTE,
                        buffer);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    void TextureArray::bind() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
    }
} // namespace EconSimPlusPlus::Engine
