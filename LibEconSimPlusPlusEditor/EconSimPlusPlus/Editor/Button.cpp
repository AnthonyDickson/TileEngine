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

#include <utility>

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Editor/Button.hpp>


namespace EconSimPlusPlus::Editor {
    Button::Button(Engine::Font*, std::string text, const glm::vec2 position, std::function<void()> callback) :
        m_text(std::move(text)), m_callback(std::move(callback)) {
        // TODO: Lift out update and render function to own interfaces
        // TODO: Create separate base class for GUI elements similar to GameObject that is treated differently (e.g., should live in different list).
        // TODO: Create TextLabel class that stores text render settings, position and dimensions.
        // TODO: Calculate text size
        // TODO: Calculate button size w/ padding.
        setTransform(glm::translate(transform(), glm::vec3{position, 0.0f}));
    }

    void Button::update(float, const Engine::InputState& inputState, const Engine::Camera& camera) {
        // ReSharper disable once CppTooWideScopeInitStatement
        const glm::vec2 point{Engine::screenToWorldCoordinates(camera, inputState.getMousePosition())};

        if (inputState.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and contains(point)) {
            m_callback();
        }
    }

    void Button::render(const Engine::Camera&) const {
        /// TODO: Draw button outline
        /// TODO: Draw button fill
        /// TODO: Draw button text
    }
} // namespace EconSimPlusPlus::Editor
