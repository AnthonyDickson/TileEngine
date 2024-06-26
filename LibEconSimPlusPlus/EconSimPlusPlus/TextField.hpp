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
// Created by Anthony Dickson on 19/06/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTFIELD_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTFIELD_HPP

#include <functional>

#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/Text.hpp>
#include <EconSimPlusPlus/TextCaret.hpp>
#include <EconSimPlusPlus/TextFieldStyle.hpp>

namespace EconSimPlusPlus {

    /// A box where the user can type in text.
    class TextField final : public Object {
    public:
        /// The possible states of a text field.
        enum class State { inactive, active };

        /// Create a text field for user text entry.
        /// @param font The font to render text with.
        /// @param style The configuration for the text field appearence.
        explicit TextField(const Font* font, const TextFieldStyle& style);

        /// Register a function to be called when the text field changes to a given state.
        /// @param state The target state in which to call the specified function.
        /// @param function The function to call when changing to the specified state.
        void setTransition(State state, const std::function<void()>& function);

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Camera& camera) const override;

    private:
        /// Chnage the text field to another state.
        /// @param state The next state.
        void transitionTo(State state);

        /// The text that is displayed and edited in the text field.
        Text m_text;
        /// The configuration for the text field appearence.
        const TextFieldStyle m_style;
        /// Shows a blinking line to indicate where the text will be inserted.
        TextCaret m_caret;
        /// The current state of the text field.
        State m_state{State::inactive};
        /// A mapping between states and their transitions.
        std::map<State, std::function<void()>> m_transitions{};

        /// The geometry used to draw the background.
        Quad m_quad{};
        Shader m_shader{Shader::create("resource/shader/grid.vert", "resource/shader/grid.frag")};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTFIELD_HPP
