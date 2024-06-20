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

#include <iostream>

#include <EconSimPlusPlus/TextField.hpp>

// TODO: Capture input while text field is active, store in text label (this will also update the display).
// TODO: Add blinking text cursor while text field is active.
// TODO: Switch to text edit cursor while mouse is hovering over text field.
namespace EconSimPlusPlus {
    TextField::TextField(const Font* font) : m_text("Foo", font, {}) {
        Object::setSize(m_text.size());
        Object::setScale(m_text.size());
        Object::setLayer(99.0f);
        m_text.setLayer(layer());
    }
    void TextField::update(float, const InputState& inputState, const Camera& camera) {
        switch (m_state) {
        case TextFieldState::inactive:
            if (inputState.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and
                contains(screenToWorldCoordinates(inputState.getMousePosition(), camera))) {
                m_state = TextFieldState::active;
                std::cout << "Text field is now active.\n";
            }
            break;
        case TextFieldState::active:
            if (inputState.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and
                not contains(screenToWorldCoordinates(inputState.getMousePosition(), camera))) {
                m_state = TextFieldState::inactive;
                std::cout << "Text field is now inactive.\n";
            }
            break;
        }
    }

    void TextField::render(const Camera& camera) const {
        m_shader.bind();
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * camera.viewMatrix());
        m_shader.setUniform("transform", transform());
        m_shader.setUniform("color", glm::vec3{0.4f});

        m_quad.render(GL_TRIANGLE_STRIP);
        m_text.render(camera);
    }
} // namespace EconSimPlusPlus
