

#include <TileEngine/Quad.hpp>


namespace TileEngine {

    Quad::Quad() {
        m_vao.bind();
        m_vbo.loadData({0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f}, {2});
    }

    void Quad::render(const GLenum mode) const {
        m_vao.bind();
        m_vbo.drawArrays(mode);
    }

    void Quad::render(const int instanceCount, const GLenum mode) const {
        m_vao.bind();
        glDrawArraysInstanced(mode, 0, 4, instanceCount);
    }
} // namespace TileEngine
