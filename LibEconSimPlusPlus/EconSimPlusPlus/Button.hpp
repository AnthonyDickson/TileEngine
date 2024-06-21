//  EconSimPlusPlus
//  A program that simulates the historical development of capitalist economies.
//  Copyright (C) 2024.   Anthony Dickson anthony.dickson9656@gmail.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by Anthony Dickson on 18/05/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_BUTTON_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_BUTTON_HPP

#include <functional>

#include <EconSimPlusPlus/ButtonStyle.hpp>
#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Text.hpp>

namespace EconSimPlusPlus {

    class Button final : public Object {
    public:
        /// Create a button.
        /// @param text The text to display in the button. Note that the text color will be overriden by the color in
        /// the button style(s).
        /// @param position Where to position the button in screen space. Uses the bottom left as the origin.
        /// @param anchor The point on the button that the position refers to.
        /// @param callback The function to call when the button is clicked.
        /// @param style Configuration for button apperance.
        /// @param activeStyle Configuration for button apperance when the button is pressed/clicked.
        /// @param disabledStyle Configuration for button apperance when the button is disabled.
        Button(const Text& text, glm::vec2 position, Anchor anchor, std::function<void()> callback,
               const ButtonStyle& style, const ButtonStyle& activeStyle = {}, const ButtonStyle& disabledStyle = {});

        /// Enable or disable the button.
        /// @note When the button is disabled, the button's callback will not be called.
        /// @param enabled Whether the button should be enabled (`true`) or disabled (`false`).
        void setEnabled(bool enabled);

        void setPosition(glm::vec2 position) override;
        void setLayer(float layer) override;
        void setAnchor(Anchor anchor) override;

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Camera& camera) const override;

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
        const ButtonStyle m_normalStyle;
        /// Configuration for button apperance when the button is pressed/clicked.
        const ButtonStyle m_activeStyle;
        /// Configuration for button apperance when the button is disabled.
        const ButtonStyle m_disabledStyle;
        /// The style currently being used.
        ButtonStyle m_currentStyle;
        /// The current state of the button.
        State m_state{State::normal};

        /// The button geometry.
        Quad m_quad{};
        Shader m_shader{Shader::create("resource/shader/grid.vert", "resource/shader/grid.frag")};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_BUTTON_HPP
