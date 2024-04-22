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

#include "glad/glad.h"

#include "Size.h"

/** Handles the creation and setup of an OpenGL texture from a image on disk.*/
class Texture {
private:
    /** The OpenGL ID for the texture.*/
    const unsigned int textureID;
    /** The texture unit that the texture should be bound to. */
    const int textureUnit;

public:
    /** The size (width, height) of the texture in pixels. */
    const Size<int> resolution;

    /**
     *
     * @param textureID_ The OpenGL ID for the texture.
     * @param textureUnit_ The texture unit to bind the texture to.
     * @param resolution_ The resolution of the texture.
     */
    Texture(unsigned int textureID_, int textureUnit_, Size<int> resolution_);

    /**
     * Create a texture from an image.
     * @param imagePath The path to an image.
     * @param textureUnit_ (optional) Which texture unit to bind. Defaults to GL_TEXTURE0.
     */
    static std::shared_ptr<Texture> create(const std::string& imagePath, int textureUnit_ = GL_TEXTURE0);

    /** Delete copy constructor to avoid OpenGL issues. */
    Texture(Texture&) = delete;
    /** Delete move constructor to avoid OpenGL issues. */
    Texture(Texture&&) = delete;

    /** Clean up OpenGL related stuff.*/
    ~Texture();

    /** Activate the current texture for bind in rendering.*/
    void bind() const;

    [[nodiscard]] int getUniformTextureUnit() const;
};


#endif // ECONSIMPLUSPLUS_TEXTURE_H
