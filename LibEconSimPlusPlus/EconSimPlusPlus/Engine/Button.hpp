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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_BUTTON_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_BUTTON_HPP

#include <functional>

#include <EconSimPlusPlus/Engine/Font.hpp>
#include <EconSimPlusPlus/Engine/GuiObject.hpp>
#include <EconSimPlusPlus/Engine/Text.hpp>

namespace EconSimPlusPlus::Engine {

    class Button final : public GUIObject {
    public:
        /// Create a button.
        /// @param text The text to display in the button.
        /// @param position Where to position the button in screen space. Uses the bottom left as the origin.
        /// @param callback The function to call when the button is clicked.
        Button(const Text& text, glm::vec2 position, std::function<void()> callback);

        void setPosition(glm::vec2 position) override;
        void setLayer(float layer) override;

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Camera& camera) const override;

    private:
        /// The text to display in the button.
        Text m_text;
        /// The function to call when the button is clicked.
        const std::function<void()> m_callback;

        // TODO: Refactor common pattern of having a VAO, VBO and shader.
        // TODO: Also refactor common shapes, e.g., a quad.
        VertexArray m_vao{};
        VertexBuffer m_vbo{};
        Shader m_shader{Shader::create("resource/shader/grid.vert", "resource/shader/grid.frag")};
    };

} // namespace EconSimPlusPlus::Engine

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_BUTTON_HPP
