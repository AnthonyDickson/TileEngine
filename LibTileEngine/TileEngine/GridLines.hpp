

#ifndef LIBTILEENGINE_TILEENGINE_GRIDLINES_HPP
#define LIBTILEENGINE_TILEENGINE_GRIDLINES_HPP

#include "glm/vec2.hpp"

#include <TileEngine/Camera.hpp>
#include <TileEngine/Object.hpp>
#include <TileEngine/Shader.hpp>
#include <TileEngine/VertexArray.hpp>
#include <TileEngine/VertexBuffer.hpp>

namespace TileEngine {
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

} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_GRIDLINES_HPP
