

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_BUTTON_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_BUTTON_HPP

#include <functional>

#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/Outline.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Text.hpp>

namespace EconSimPlusPlus {

    class Button final : public Object {
    public:
        /// Configuration for button appearance.
        struct Style {
            /// The horizontal and vertical space between the inner borders of the button and its child objects in
            /// pixels.
            glm::vec2 padding{16.0f};
            /// The color of the button's text.
            glm::vec3 textColor = glm::vec3{0.0f};
            /// The color to fill the background of the button with.
            glm::vec3 fillColor = glm::vec3{1.0f};
            /// The configuration for the appearance of the outline.
            Outline::Style outline{
                .color = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, .thickness = 0.0f, .placement = Outline::Placement::inset};
        };

        /// Create a button.
        /// @param text The text to display in the button. Note that the text color will be overriden by the color in
        /// the button style(s).
        /// @param callback The function to call when the button is clicked.
        /// @param style Configuration for button apperance.
        /// @param activeStyle Configuration for button apperance when the button is pressed/clicked.
        /// @param disabledStyle Configuration for button apperance when the button is disabled.
        Button(const Text& text, std::function<void()> callback, const Style& style, const Style& activeStyle,
               const Style& disabledStyle);

        /// Activate the button and allow the user to interact with it.
        void enable();

        /// Deactivate the button stop the user from interacting with it.
        void disable();

        void setPosition(glm::vec2 position) override;
        void setLayer(float layer) override;
        void setAnchor(Anchor anchor) override;

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Graphics& graphics) const override;

    private:
        /// The state of a button.
        enum class State { normal, active, disabled };

        /// Set the state of the button.
        /// @param state The state to put the button into.
        void setState(State state);

        /// Update the style based on the current button state.
        void updateStyle();

        /// The default event handler for a button.
        /// @param event What happened.
        /// @param window The window where the event happened.
        void handleEvents(Event event, Window& window);

        /// The text to display in the button.
        Text m_text;
        /// The function to call when the button is clicked.
        const std::function<void()> m_callback;
        /// Configuration for button apperance.
        const Style m_normalStyle;
        /// Configuration for button apperance when the button is pressed/clicked.
        const Style m_activeStyle;
        /// Configuration for button apperance when the button is disabled.
        const Style m_disabledStyle;
        /// The style currently being used.
        Style m_currentStyle;
        /// The current state of the button.
        State m_state{State::normal};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_BUTTON_HPP
