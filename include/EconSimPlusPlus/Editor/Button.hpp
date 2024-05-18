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

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <functional>

#include <EconSimPlusPlus/Engine/Font.hpp>
#include <EconSimPlusPlus/Engine/GameObject.hpp>

namespace EconSimPlusPlus::Editor {

    class Button final : public Engine::GameObject {
    public:
        /// Create a button.
        /// @param font The font to use for rendering the button text.
        /// @param text The text to display in the button.
        /// @param position Where to position the button in world space. Uses the bottom left as the origin.
        /// @param callback The function to call when the button is clicked.
        Button(Engine::Font* font, std::string text, glm::vec2 position, std::function<void()> callback);

        void update(float deltaTime, const Engine::InputState& inputState, const Engine::Camera& camera) override;
        void render(const Engine::Camera& camera) const override;

    private:
        /// The text to display in the button.
        const std::string m_text;
        /// The function to call when the button is clicked.
        const std::function<void()> m_callback;
    };

} // namespace EconSimPlusPlus::Editor
// EconSimPlusPlus

#endif // BUTTON_HPP
