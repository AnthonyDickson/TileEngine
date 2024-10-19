

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_GRIDLINES_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_GRIDLINES_HPP

#include "glm/vec2.hpp"

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {
    /// Draws 2D grid lines.
    class GridLines final : public Object {
    public:
        /// Create a grid lines object.
        /// @param size The width and height of the grid in tiles.
        /// @param cellSize The width and height of the cells in pixels.
        GridLines(glm::ivec2 size, glm::vec2 cellSize);

        GridLines(GridLines&) = delete; // Prevent copy to avoid issues w/ OpenGL

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Graphics& graphics) const override;

    private:
        /// The vertex array object.
        const VertexArray m_vao{};
        /// The vertex buffer object.
        VertexBuffer m_vbo{};
        /// The shader for drawing grid lines.
        const Shader m_shader{Shader::create("resource/shader/grid.vert", "resource/shader/grid.frag")};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_GRIDLINES_HPP
