

//
// Created by Anthony on 1/04/2024.
//

#include <EconSimPlusPlus/VertexArray.hpp>

namespace EconSimPlusPlus {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_id);
    }

    void VertexArray::bind() const {
        glBindVertexArray(m_id);
    }
} // namespace EconSimPlusPlus
