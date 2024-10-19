

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTCARET_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTCARET_HPP

#include <EconSimPlusPlus/Object.hpp>

namespace EconSimPlusPlus {
    /// Shows a blinking line to indicate where the text will be inserted.
    class TextCaret final : public Object {
    public:
        /// The configuration for the appearance of a text cursor.
        struct Style {
            /// The color of the blinking text cursor.
            glm::vec3 color{0.0f};
            /// The width of the blinking text cursor in pixels.
            float width{2.0f};
        };

        explicit TextCaret(Style style);


        /// Set the height of the caret in pixels.
        /// @note The width is set through the style that is passed into the constructor.
        void setHeight(float height);

        void setSize(glm::vec2 size) override;

        /// Stop rendering the caret.
        void hide();
        /// Start rendering the caret.
        void show();

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Graphics& graphics) const override;

    private:
        /// The possible states of a text caret.
        enum class State { visible, hidden };

        const Style m_style;
        /// The current state of the text caret.
        State m_state{State::visible};
        /// The time since the caret was shown. The transparency (alpha) is calculated as a function of time to create a
        /// blinking effect.
        float m_time{0.0f};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTCARET_HPP
