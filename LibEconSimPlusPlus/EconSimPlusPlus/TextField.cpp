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

// TODO: Add blinking text cursor while text field is active.
// TODO: Switch to text edit cursor while mouse is hovering over text field.
namespace EconSimPlusPlus {
    namespace {
        /// GLFW keycodes for number keys.
        constexpr std::array numeric = {
            GLFW_KEY_0, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4,
            GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9,
        };

        /// GLFW keycodes for alphabet keys.
        constexpr std::array alpha = {
            GLFW_KEY_A, GLFW_KEY_B, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_I,
            GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_M, GLFW_KEY_N, GLFW_KEY_O, GLFW_KEY_P, GLFW_KEY_Q, GLFW_KEY_R,
            GLFW_KEY_S, GLFW_KEY_T, GLFW_KEY_U, GLFW_KEY_V, GLFW_KEY_W, GLFW_KEY_X, GLFW_KEY_Y, GLFW_KEY_Z};
    } // namespace

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
            }
            break;
        case TextFieldState::active:
            if (inputState.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and
                not contains(screenToWorldCoordinates(inputState.getMousePosition(), camera))) {
                m_state = TextFieldState::inactive;
            }

            for (const int& key : numeric) {
                if (inputState.getKeyDown(key)) {
                    m_text.setText(m_text.text() + static_cast<char>(key));
                }
            }

            for (const int& key : alpha) {
                if (inputState.getKeyDown(key)) {
                    const char character{static_cast<char>(inputState.getKey(GLFW_KEY_LEFT_SHIFT) ? key : key + 32)};
                    m_text.setText(m_text.text() + character);
                }
            }

            if (inputState.getKeyDown(GLFW_KEY_BACKSPACE)) {
                m_text.setText(m_text.text().substr(0, m_text.text().length() - 1));
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
