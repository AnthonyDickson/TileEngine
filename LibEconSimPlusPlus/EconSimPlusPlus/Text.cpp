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
// Created by Anthony Dickson on 21/05/2024.
//

#include <EconSimPlusPlus/Text.hpp>

namespace EconSimPlusPlus {
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
} // namespace EconSimPlusPlus
