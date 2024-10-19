

//
// Created by Anthony on 1/04/2024.
//

#include <numeric>

#include <TileEngine/VertexBuffer.hpp>

namespace TileEngine {
    VertexBuffer::VertexBuffer() {
        glGenBuffers(1, &m_id);
    }

    void VertexBuffer::loadData(const std::vector<float>& vertexData, const std::vector<int>& sizes) {
        bind();

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexData.size() * sizeof(float)), vertexData.data(),
                     GL_STATIC_DRAW);

        const int stride{std::reduce(sizes.begin(), sizes.end(), 0)};
        const int strideBytes{stride * static_cast<int>(sizeof(float))};
        int offset{0};

        for (std::size_t i = 0; i < sizes.size(); i++) {
            const int size = sizes[i];
            const auto pointerOffset{reinterpret_cast<void*>(offset * sizeof(float))};
            glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, strideBytes, pointerOffset);
            glEnableVertexAttribArray(i);
            offset += size;
        }

        m_vertexCount = static_cast<int>(vertexData.size()) / stride;
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::drawArrays(const GLenum mode) const {
        glDrawArrays(mode, 0, m_vertexCount);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_id);
    }
} // namespace TileEngine
