

#ifndef LIBTILEENGINE_TILEENGINE_TEXTFIELD_HPP
#define LIBTILEENGINE_TILEENGINE_TEXTFIELD_HPP

#include <functional>

#include <TileEngine/Object.hpp>
#include <TileEngine/Outline.hpp>
#include <TileEngine/Text.hpp>
#include <TileEngine/TextCaret.hpp>

namespace TileEngine {

    /// A box where the user can type in text.
    class TextField final : public Object {
    public:
        /// The possible states of a text field.
        enum class State { inactive, active };

        /// What inputs the text field should accept.
        enum class Mode {
            /// Alphabet a-z
            alpha = 1 << 0,
            /// Digits 0-9
            numeric = 1 << 1,
            /// Alphabet and digits (a-z, 0-9)
            alphanumeric = alpha | numeric
        };

        /// The configuration for the text field behavior.
        struct Config {
            /// The maximum number of characters. Zero or less indicates infinite length.
            int maxLength{32};
            /// What inputs the text field should accept.
            Mode mode{Mode::alphanumeric};
        };

        /// The configuration for the appearance of a text field.
        struct Style {
            /// The color with which to fill the text field background.
            glm::vec3 fillColor{1.0f};
            /// The horizontal and vertical space between the inner borders of the text field and its child objects in
            /// pixels.
            glm::vec2 padding{8.0f};
            /// The color of the text inside the text field.
            Font::Style text{};
            /// The color the placeholder text when no text has been entered.
            glm::vec3 placeholderTextColor{0.5f};
            /// The style of the blinking text cursor.
            TextCaret::Style caret{.color = glm::vec3{0.0f}, .width = 2.0f};
            /// The configuration for the appearance of the outline during the inactive state.
            Outline::Style outlineInactive{
                .color = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, .thickness = 1.0f, .placement = Outline::Placement::outset};
            /// The configuration for the appearance of the outline during the active state.
            Outline::Style outlineActive{
                .color = glm::vec4{0.0f, 0.5f, 1.0f, 1.0f}, .thickness = 1.0f, .placement = Outline::Placement::outset};
        };

        using SubmitAction = std::function<void(const std::string& text)>;
        using InputValidator = std::function<bool(const std::string& text)>;

        /// Create a text field for user text entry.
        /// @param placeholder The text to display initially.
        /// @param font The font to render text with.
        /// @param config The configuration for the text field behavior.
        /// @param style The configuration for the text field appearence.
        explicit TextField(const std::string& placeholder, const Font* font, const Config& config, const Style& style);

        /// Set the function to call to check whether the input text is valid.
        /// @param function A function that takes a string and returns a bool.
        void setInputValidator(const InputValidator& function);

        /// Set the function to call when the text field is submitted (user presses enter key while the text field is
        /// active).
        /// @param function A function that takes a string.
        void setSubmitAction(const SubmitAction& function);

        /// Get the text field's text.
        /// @return A string.
        [[nodiscard]] std::string text() const;

        /// Update the text.
        /// @param text The text to display.
        void setText(const std::string& text);

        void setPosition(glm::vec2 position) override;
        void setLayer(float layer) override;
        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Graphics& graphics) const override;

    private:
        /// Change the text field to another state.
        /// @param state The next state.
        void transitionTo(State state);

        /// The text that is displayed and edited in the text field.
        Text m_text;
        /// The placeholder text to show when the text field is empty.
        Text m_placeholder;
        /// The configuration for the text field behavior.
        const Config m_config;
        /// The configuration for the text field appearence.
        const Style m_style;
        /// Shows a blinking line to indicate where the text will be inserted.
        TextCaret m_caret;
        /// The current state of the text field.
        State m_state{State::inactive};
        /// The function to call to check whether the input text is valid.
        InputValidator m_inputValidator{};
        /// The function to call when the text field is submitted (user presses enter key while the text field is
        /// active).
        SubmitAction m_submitAction{};
        /// A backup of the text field's text before it entered the active state.
        /// The text is reverted to this value if the user tries to submit an invalid value.
        std::string m_rollbackString{};
    };

} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_TEXTFIELD_HPP
