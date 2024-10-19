

//
// Created by Anthony on 24/03/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTURE_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTURE_HPP

#include <memory>

#include "glad/glad.h"

#include <EconSimPlusPlus/Image.hpp>

namespace EconSimPlusPlus {
    /// Handles the creation and setup of an OpenGL texture from a image on disk.
    class Texture {
    public:
        /// Create a texture from an image file path.
        /// @param imagePath The path to an image.
        /// @param textureUnit Which texture unit to bind. Defaults to GL_TEXTURE0.
        static std::unique_ptr<Texture> create(const std::string& imagePath, int textureUnit = GL_TEXTURE0);

        /// Create a texture from an image.
        /// @param image The image data.
        /// @param textureUnit Which texture unit to bind. Defaults to GL_TEXTURE0.
        static std::unique_ptr<Texture> create(const Image::Image& image, int textureUnit = GL_TEXTURE0);

        /// @param textureID The OpenGL ID for the texture.
        /// @param textureUnit The texture unit to bind the texture to.
        /// @param resolution The resolution of the texture.
        /// @param path Where the texture was loaded from.
        Texture(unsigned int textureID, int textureUnit, glm::ivec2 resolution, const std::string& path = "");

        /// Delete copy constructor to avoid OpenGL issues.
        Texture(Texture&) = delete;

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
