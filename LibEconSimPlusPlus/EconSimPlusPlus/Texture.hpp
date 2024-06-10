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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTURE_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTURE_HPP

#include <memory>

#include "glad/glad.h"

namespace EconSimPlusPlus {
    /// Handles the creation and setup of an OpenGL texture from a image on disk.
    class Texture {
    public:
        /// Create a texture from an image file path.
        /// @param imagePath The path to an image.
        /// @param textureUnit (optional) Which texture unit to bind. Defaults to GL_TEXTURE0.
        static std::unique_ptr<Texture> create(const std::string& imagePath, int textureUnit = GL_TEXTURE0);

        /// Create a texture from an image.
        /// @param image The image data.
        /// @param resolution The width and height of the image data in pixels.
        /// @param channelCount The number of channels in the image data.
        /// @param textureUnit Which texture unit to bind. Defaults to GL_TEXTURE0.
        /// @param path Where the texture was loaded from.
        static std::unique_ptr<Texture> create(const unsigned char* image, glm::ivec2 resolution, int channelCount,
                                               int textureUnit = GL_TEXTURE0, const std::string& path = "");

        /// @param textureID The OpenGL ID for the texture.
        /// @param textureUnit The texture unit to bind the texture to.
        /// @param resolution The resolution of the texture.
        /// @param path Where the texture was loaded from.
        Texture(unsigned int textureID, int textureUnit, glm::ivec2 resolution, const std::string& path = "");

        /// Delete copy constructor to avoid OpenGL issues.
        Texture(Texture&) = delete;
        /// Delete move constructor to avoid OpenGL issues.
        Texture(Texture&&) = delete;

        /// Clean up OpenGL related stuff.
        ~Texture();

        /// Get the path to the image file used to create this texture.
        /// @note Potentially empty if the texture was not directly created from a file path.
        [[nodiscard]] std::string path() const;

        /// Get the size (width, height) of the texture in pixels.
        [[nodiscard]] glm::ivec2 resolution() const;

        /// Activate the current texture for bind in rendering.
        void bind() const;

        [[nodiscard]] int getUniformTextureUnit() const;
    private:
        /// The OpenGL ID for the texture.
        const unsigned int m_textureID;
        /// The texture unit that the texture should be bound to.
        const int m_textureUnit;
        /// The size (width, height) of the texture in pixels.
        const glm::ivec2 m_resolution;
        /// Where the texture was loaded from.
        const std::string m_path;

    };
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTURE_HPP
