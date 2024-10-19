

//
// Created by Anthony on 1/04/2024.
//

#ifndef LIBTILEENGINE_TILEENGINE_VERTEXARRAY_HPP
#define LIBTILEENGINE_TILEENGINE_VERTEXARRAY_HPP

#include "glad/glad.h"

namespace TileEngine {
    /// Wrapper for OpenGL vertex array objects.
    class VertexArray {
    public:
        /// Create a vertex array object.
        VertexArray();

        /// Delete copy constructor to avoid OpenGL issues.
        VertexArray(VertexArray&) = delete;
        /// Delete move constructor to avoid OpenGL issues.
        VertexArray(VertexArray&&) = delete;

        /// Cleanup OpenGL stuff.
        ~VertexArray();

        /// Bind the vertex array object.
        void bind() const;

    private:
        /// The ID for the bound vertex array object.
        GLuint m_id{};
    };
} // namespace TileEngine


#endif // LIBTILEENGINE_TILEENGINE_VERTEXARRAY_HPP
