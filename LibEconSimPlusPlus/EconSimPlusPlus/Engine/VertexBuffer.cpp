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
// Created by Anthony on 1/04/2024.
//

#include <numeric>

#include <EconSimPlusPlus/Engine/VertexBuffer.hpp>

namespace EconSimPlusPlus::Engine {
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
} // namespace EconSimPlusPlus::Engine
