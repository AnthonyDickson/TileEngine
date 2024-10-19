

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_QUAD_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_QUAD_HPP

#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {
    /// A generic quad of unit size.
    /// @note The origin is at the bottom left corner. You should use a shader to transform (e.g., scale, shift) the
    /// vertices.
    class Quad {
    public:
        Quad();
        Quad(Quad&) = delete; // Prevent issues with OpenGL stuff.

        /// Draw the quad.
        /// @param mode How to draw the vertex data.
        void render(GLenum mode = GL_TRIANGLE_STRIP) const;

        /// Draw the quad with instancing.
        /// @param instanceCount The number of instances to render.
        /// @param mode How to draw the vertex data.
        void render(int instanceCount, GLenum mode = GL_TRIANGLE_STRIP) const;

    private:
        VertexArray m_vao{};
        VertexBuffer m_vbo{};
    };
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_QUAD_HPP
