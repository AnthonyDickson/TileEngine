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

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const std::vector<float> &vertexData, int stride, const std::vector<int> &sizes)
        : triangleCount(static_cast<int>(vertexData.size()) / stride) {
    glGenBuffers(1, &vboID);
    bind();

    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertexData.size() * sizeof(float)), vertexData.data(),
                 GL_STATIC_DRAW);

    const int strideBytes = static_cast<int>(stride * sizeof(float));
    int offset{0};

    for (int i = 0; i < sizes.size(); i++) {
        int size = sizes[i];
        auto pointerOffset{(void *) (offset * sizeof(float))};
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, strideBytes, pointerOffset);
        glEnableVertexAttribArray(i);
        offset += size;
    }
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
}

void VertexBuffer::drawArrays(GLenum mode) const {
    glDrawArrays(mode, 0, triangleCount);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &vboID);
}
