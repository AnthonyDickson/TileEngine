

#include <format>
#include <iostream>
#include <vector>

#include "glm/ext/matrix_transform.hpp"

#include <TileEngine/GridLines.hpp>


namespace TileEngine {
    GridLines::GridLines(const glm::ivec2 size, const glm::vec2 cellSize) {
        std::vector<float> vertices{};
        const auto scaledSize{static_cast<glm::vec2>(size) * cellSize};

        // Horizontal Lines
        for (int row = 0; row <= size.y; ++row) {
            const float y{static_cast<float>(row) * cellSize.y};
            // Start Point
            vertices.push_back(0);
            vertices.push_back(y);
            // End Point
            vertices.push_back(scaledSize.x);
            vertices.push_back(y);
        }

        // Vertical Lines
        for (int col = 0; col <= size.x; ++col) {
            const float x{static_cast<float>(col) * cellSize.x};
            // Start Point
            vertices.push_back(x);
            vertices.push_back(0);
            // End Point
            vertices.push_back(x);
            vertices.push_back(scaledSize.y);
        }

        m_vao.bind();
        m_vbo.bind();
        m_vbo.loadData(vertices, {2});

        Object::setPosition(-0.5f * scaledSize);
        Object::setSize(scaledSize);
    }

    void GridLines::update(float, const InputState&, const Camera&) {
    }

    void GridLines::render(const Graphics& graphics) const {
        const glm::mat4 transform{
            glm::scale(glm::translate(glm::mat4{1.0f}, glm::vec3{bottomLeft(*this), layer()}), glm::vec3{1.0f})};

        m_shader.bind();
        m_shader.setUniform("color", glm::vec3{1.0f});
        m_shader.setUniform("projectionViewMatrix", projectionViewMatrix(graphics.camera));
        m_shader.setUniform("transform", transform);
        m_vao.bind();
        m_vbo.drawArrays(GL_LINES);
    }
} // namespace TileEngine
