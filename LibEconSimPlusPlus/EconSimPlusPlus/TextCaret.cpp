

#include <cmath>

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/TextCaret.hpp>

namespace EconSimPlusPlus {
    TextCaret::TextCaret(const Style style) : m_style(style) {
        Object::setSize({m_style.width, 0.0f});
    }

    void TextCaret::setHeight(float height) {
        Object::setSize({m_style.width, height});
    }

    void TextCaret::setSize(glm::vec2) {
        assert(false && "Should not set size on text caret directly. Use the setter for height instead.");
    }

    void TextCaret::hide() {
        m_state = State::hidden;
    }

    void TextCaret::show() {
        m_state = State::visible;
        m_time = 0.0f;
    }

    void TextCaret::update(const float deltaTime, const InputState&, const Camera&) {
        m_time += deltaTime;
    }

    void TextCaret::render(const Graphics& graphics) const {
        if (m_state == State::hidden) {
            return;
        }

        graphics.quadShader.bind();
        graphics.quadShader.setUniform("projectionViewMatrix", projectionViewMatrix(graphics.camera));
        graphics.quadShader.setUniform(
            "transform",
            glm::scale(glm::translate(glm::mat4{1.0f}, glm::vec3{position(), layer()}), glm::vec3{size(), 1.0f}));
        const float alpha{0.5f * (std::sin(2.0f * m_time) + 1.0f)};
        graphics.quadShader.setUniform("color", glm::vec4{m_style.color, alpha});
        graphics.quad.render();
    }
} // namespace EconSimPlusPlus
