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

#include <iostream>

#include "glad/glad.h"
#include "stb_image.h"

#include "Texture.h"

Texture::Texture(const std::string &imagePath, unsigned int textureUnit, unsigned int imageFormat) :
        textureID(0), textureUnit(textureUnit) {
    glGenTextures(1, &textureID);
    use();

    int width{};
    int height{};
    int channelCount{};
    unsigned char *imageData = stbi_load(imagePath.c_str(), &width, &height, &channelCount, 0);

    if (imageData == nullptr) {
        std::cout << "Texture failed to load image from " << imagePath << ":\n" << stbi_failure_reason()
                  << std::endl;
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(imageData);
}

void Texture::use() const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::cleanup() {
    glDeleteTextures(1, &textureID);
}