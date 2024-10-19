

#include <TileEngine/Text.hpp>

namespace TileEngine {
    Text::Text(const std::string& text, const Font* font, const Font::Style& style) :
        m_font(font), m_style(style) {
        setText(text);
    }

    std::string Text::text() const {
        return m_text;
    }

    void Text::setText(const std::string& text) {
        m_text = text;

        const float scale{m_font->calculateScaleFactor(m_style)};
        setSize(m_font->calculateTextSize(m_text) * scale);
    }

    void Text::setColor(const glm::vec3 color) {
        m_style.color = color;
    }

    void Text::update(float, const InputState&, const Camera&) {
    }

    void Text::render(const Graphics& graphics) const {
        m_font->render(m_text, {position(), layer()}, anchor(), m_style, graphics.camera);
    }
} // namespace TileEngine
