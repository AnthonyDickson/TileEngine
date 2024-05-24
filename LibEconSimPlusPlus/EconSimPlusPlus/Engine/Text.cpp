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

#include <utility>

#include <EconSimPlusPlus/Engine/Text.hpp>

namespace EconSimPlusPlus::Engine {
    Text::Text(std::string text, const Engine::Font* font, const Engine::Font::RenderSettings& settings) :
        m_text(std::move(text)), m_font(font), m_settings(settings) {
        // TODO: Refactor common code.
        const float scale{m_settings.size / m_font->fontHeight()};
        setSize(m_font->calculateTextSize(m_text) * scale + m_settings.padding);
    }

    void Text::setText(const std::string& text) {
        m_text = text;

        const float scale{m_settings.size / m_font->fontHeight()};
        setSize(m_font->calculateTextSize(text) * scale + m_settings.padding);
    }

    void Text::update(float, const Engine::InputState&, const Engine::Camera&) {
    }

    void Text::render(const Engine::Camera& camera) const {
        m_font->render(m_text, {position(), layer()}, camera, m_settings);
    }
} // namespace EconSimPlusPlus::Engine
