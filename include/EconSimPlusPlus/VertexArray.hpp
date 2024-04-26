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

#ifndef ECONSIMPLUSPLUS_VERTEXARRAY_H
#define ECONSIMPLUSPLUS_VERTEXARRAY_H


#include "glad/glad.h"

/** Wrapper for OpenGL vertex array objects. */
class VertexArray {
private:
    /** The ID for the bound vertex array object. */
    GLuint vaoID{};
public:
    /** Create a vertex array object. */
    VertexArray();

    /** Delete copy constructor to avoid OpenGL issues. */
    VertexArray(VertexArray &) = delete;
    /** Delete move constructor to avoid OpenGL issues. */
    VertexArray(VertexArray &&) = delete;

    /** Cleanup OpenGL stuff. */
    ~VertexArray();

    /** Bind the vertex array object. */
    void bind() const;
};


#endif //ECONSIMPLUSPLUS_VERTEXARRAY_H
