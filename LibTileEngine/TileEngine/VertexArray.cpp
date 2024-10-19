

//
// Created by Anthony on 1/04/2024.
//

#include <TileEngine/VertexArray.hpp>

namespace TileEngine {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArray::bind() const {
        glBindVertexArray(m_id);
    }
} // namespace TileEngine
