

//
// Created by Anthony on 24/03/2024.
//

#include <format>

#include <glm/vec2.hpp>

#include <TileEngine/Texture.hpp>

namespace TileEngine {

    std::unique_ptr<Texture> Texture::create(const std::string& imagePath, const int textureUnit) {
        return std::unique_ptr{create(Image::create(imagePath), textureUnit)};
    }

    std::unique_ptr<Texture> Texture::create(const Image::Image& image, const int textureUnit) {
        GLenum imageFormat;

        switch (image.channels) {
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
            throw std::runtime_error(std::format("Texture does not support image with {0} channels.", image.channels));
        }

        GLuint textureID{};
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(imageFormat), image.resolution.x, image.resolution.y, 0,
                     imageFormat, GL_UNSIGNED_BYTE, image.bytes.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return std::make_unique<Texture>(textureID, textureUnit, image.resolution, image.path);
    }

    Texture::Texture(const unsigned int textureID, const int textureUnit, const glm::ivec2 resolution,
                     const std::string& path) : // NOLINT(*-pass-by-value)
        m_textureID(textureID), m_textureUnit(textureUnit), m_resolution(resolution), m_path(path) {
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_textureID);
    }

    std::string Texture::path() const {
        return m_path;
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
} // namespace TileEngine
