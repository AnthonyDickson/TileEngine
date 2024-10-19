

#include "glad/glad.h"

#include <TileEngine/TextureArray.hpp>

namespace TileEngine {
    TextureArray::TextureArray(const unsigned int id) : m_id(id) {
    }

    TextureArray::~TextureArray() {
        glDeleteTextures(1, &m_id);
    }

    std::unique_ptr<TextureArray> TextureArray::create(const int depth, const glm::ivec2 resolution) {
        unsigned int textureArrayID;
        glGenTextures(1, &textureArrayID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, resolution.x, resolution.y, depth, 0, GL_RED, GL_UNSIGNED_BYTE,
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
} // namespace TileEngine
