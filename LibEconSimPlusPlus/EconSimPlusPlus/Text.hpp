

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXT_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXT_HPP

#include "glm/vec3.hpp"

#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/Object.hpp>

namespace EconSimPlusPlus {
    /// A GUI element that displays text.
    class Text final : public Object {
    public:
        /// Create a text label.
        /// @param text The text to display.
        /// @param font The font to use to display the text.
        /// @param style The configuration (e.g., color, anchor, outlines) to use for displaying the text.
        Text(const std::string& text, const Font* font, const Font::Style& style);

        /// Get the label's text.
        /// @return a string.
        [[nodiscard]] std::string text() const;

        /// Update the text.
        /// @param text The text to display.
        void setText(const std::string& text);

        /// Update the text color.
        /// @param color An RGB color with values between 0.0 and 1.0 inclusive.
        void setColor(glm::vec3 color);

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Graphics& graphics) const override;

    private:
        /// The text to display.
        std::string m_text{};
        /// The font to use to display the text.
        const Font* m_font;
        /// The configuration (e.g., color, anchor, outlines) to use for displaying the text.
        Font::Style m_style;
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXT_HPP
