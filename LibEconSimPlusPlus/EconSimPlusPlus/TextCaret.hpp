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
// Created by Anthony Dickson on 25/06/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTCARET_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTCARET_HPP

#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Shader.hpp>

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

        /// Stop rendering the caret.
        void hide();
        /// Start rendering the caret.
        void show();

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Camera& camera) const override;

    private:
        /// The possible states of a text caret.
        enum class State { visible, hidden };

        const Style m_style;
        /// The current state of the text caret.
        State m_state{State::visible};
        /// The time since the caret was shown. The transparency (alpha) is calculated as a function of time to create a
        /// blinking effect.
        float m_time{0.0f};

        Shader m_shader{Shader::create("resource/shader/grid.vert", "resource/shader/rgba.frag")};
        Quad m_quad{};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTCARET_HPP
