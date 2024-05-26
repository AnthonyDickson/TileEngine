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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_TEXT_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_TEXT_HPP

#include <EconSimPlusPlus/Engine/Font.hpp>
#include <EconSimPlusPlus/Engine/GuiObject.hpp>

namespace EconSimPlusPlus::Engine {
    /// A GUI element that displays text.
    class Text final : public GUIObject {
    public:
        /// Create a text label.
        /// @param text The text to display.
        /// @param font The font to use to display the text.
        /// @param settings The configuration (e.g., color, anchor, outlines) to use for displaying the text.
        Text(const std::string& text, const Font* font, const FontSettings& settings);

        /// Update the text.
        /// @param text The text to display.
        void setText(const std::string& text);

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Camera& camera) const override;

    private:
        /// The text to display.
        std::string m_text{};
        /// The font to use to display the text.
        const Font* m_font;
        /// The configuration (e.g., color, anchor, outlines) to use for displaying the text.
        const FontSettings m_settings;
    };

} // namespace EconSimPlusPlus::Engine

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_TEXT_HPP
