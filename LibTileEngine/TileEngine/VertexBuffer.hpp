

//
// Created by Anthony on 1/04/2024.
//

#ifndef LIBTILEENGINE_TILEENGINE_VERTEXBUFFER_HPP
#define LIBTILEENGINE_TILEENGINE_VERTEXBUFFER_HPP

#include <vector>

#include "glad/glad.h"

namespace TileEngine {
    /// Wrapper for OpenGL vertex buffer objects.
    class VertexBuffer {
    public:
        /// Create a vertex buffer object.
        VertexBuffer();

        /// Delete copy constructor to avoid OpenGL issues.
        VertexBuffer(VertexBuffer&) = delete;
        /// Delete move constructor to avoid OpenGL issues.
        VertexBuffer(VertexBuffer&&) = delete;

        /// Cleanup OpenGL stuff.
        ~VertexBuffer();

        /// Load vertex data into the vertex buffer.
        /// @param vertexData The vertex data as a flat list.
        /// @param sizes The number of elements per vertex attribute.
        void loadData(const std::vector<float>& vertexData, const std::vector<int>& sizes);

        /// Bind the vertex buffer object.
        void bind() const;

        /// Call OpenGL::glDrawArrays with suitable parameters.
        /// @param mode How to draw the vertex data.
        void drawArrays(GLenum mode = GL_TRIANGLES) const;

    private:
        /// The ID for the vertex buffer object.
        GLuint m_id{};
        /// The number of vertices in the buffered vertex data.
        int m_vertexCount{};
    };
} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_VERTEXBUFFER_HPP
