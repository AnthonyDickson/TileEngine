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

#ifndef ECONSIMPLUSPLUS_VERTEXBUFFER_H
#define ECONSIMPLUSPLUS_VERTEXBUFFER_H


#include <vector>
#include "glad/glad.h"

/** Wrapper for OpenGL vertex buffer objects. */
class VertexBuffer {
private:
    /** The ID for the vertex buffer object. */
    GLuint vboID{};
    /** The number of triangles in the buffered vertex data. */
    int triangleCount{};
public:
    /** Create a vertex buffer object. */
    VertexBuffer();

    /** Delete copy constructor to avoid OpenGL issues. */
    VertexBuffer(VertexBuffer &) = delete;
    /** Delete move constructor to avoid OpenGL issues. */
    VertexBuffer(VertexBuffer &&) = delete;

    /** Cleanup OpenGL stuff. */
    ~VertexBuffer();

    /**
     * Load vertex data into the vertex buffer.
     * @param vertexData The vertex data as a flat list.
     * @param sizes The number of elements per vertex attribute.
     */
    void loadData(const std::vector<float>& vertexData, const std::vector<int>& sizes);

    /** Bind the vertex buffer object. */
    void bind() const;

    /**
     * Call OpenGL::glDrawArrays with suitable parameters.
     * @param mode How to draw the vertex data.
     */
    void drawArrays(GLenum mode = GL_TRIANGLES) const;
};


#endif //ECONSIMPLUSPLUS_VERTEXBUFFER_H
