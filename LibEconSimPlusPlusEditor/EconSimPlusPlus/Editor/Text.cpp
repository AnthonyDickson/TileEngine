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

#include <EconSimPlusPlus/Editor/Text.hpp>


namespace EconSimPlusPlus::Editor {
    Text::Text(const std::string& text, Engine::Font* font, Engine::Font::RenderSettings settings, glm::vec2 padding) :
        m_text(text), m_font(font), m_settings(settings), m_padding(padding) {
        // TODO: Add scale factor which is used to scale the below size and padding used for rendering.
        setSize(m_font->calculateTextSize(m_text) + m_padding);
    }

    void Text::update(float, const Engine::InputState&, const Engine::Camera&) {
    }

    void Text::render(const Engine::Camera& camera) const {
        m_font->render(m_text, {position() + m_padding / 2.0f, layer()}, camera, m_settings);
    }
} // namespace EconSimPlusPlus::Editor
