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

#ifndef ECONSIMPLUSPLUS_TEXTURE_H
#define ECONSIMPLUSPLUS_TEXTURE_H


#include <string>

/** Handles the creation and setup of an OpenGL texture from a image on disk.*/
class Texture {
private:
    /** The OpenGL ID for the texture.*/
    unsigned int textureID;
    /** The texture unit that the texture should be bound to. */
    unsigned int textureUnit;

public:
    /**
     * Create a texture from an image.
     * @param imagePath The path to an image.
     * @param textureUnit_ (optional) Which texture unit to use. Defaults to GL_TEXTURE0.
     * @param imageFormat_ (optional) The image format. Defaults to GL_RGB.
     */
    explicit Texture(const std::string &imagePath, unsigned int textureUnit_ = GL_TEXTURE0,
                     unsigned int imageFormat_ = GL_RGB);

    /** Activate the current texture for use in rendering.*/
    void use() const;

    /** Clean up OpenGL related stuff.*/
    void cleanup();
};


#endif //ECONSIMPLUSPLUS_TEXTURE_H
