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

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Outline.hpp>
#include <EconSimPlusPlus/TextField.hpp>

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

    TextField::TextField(const Font* font) : m_text("Foo", font, {.color = glm::vec3{0.0f}}) {
        // TODO: Wrap configuration options (e.g., padding, caret color) into struct.
        m_padding = glm::vec2{8.0f};
        Object::setSize(m_text.size() + m_padding);
        Object::setScale(m_text.size() + m_padding);
        Object::setLayer(50.0f);

        // TODO: Hook into setters to sync settings of child objects.
        m_text.setAnchor(Anchor::topLeft);
        m_text.setPosition(topLeft(*this) + 0.5f * glm::vec2{m_padding.x, -m_padding.y});
        m_text.setLayer(layer());
        m_caret.setLayer(layer());
        m_caret.setSize(glm::vec2{2.0f, m_text.size().y});
        m_caret.hide();

        addEventHandler([&](const Event event, const EventData& eventData) {
            switch (event) {
            case Event::mouseEnter:
                eventData.window.setCursor(GLFW_IBEAM_CURSOR);
                break;
            case Event::mouseLeave:
                eventData.window.setCursor(GLFW_CURSOR_NORMAL);
                break;
            case Event::mouseClick:
                transitionTo(State::active);
                break;
            default:
                break;
            }
        });
    }

    void TextField::setTransition(const State state, const std::function<void()>& function) {
        m_transitions[state] = function;
    }

    void TextField::update(const float deltaTime, const InputState& inputState, const Camera& camera) {
        switch (m_state) {
        case State::inactive:
            break;
        case State::active:
            // TODO: Also transition to inactive state if escape key is pressed. The escape key press should not
            // propagate to other parts of the program.
            if (inputState.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and
                not contains(screenToWorldCoordinates(inputState.mousePosition(), camera))) {
                transitionTo(State::inactive);
                break;
            }

            // TODO: Move keyboard input logic into event handler(s)?
            for (const int& key : numeric) {
                if (inputState.keyDown(key)) {
                    m_text.setText(m_text.text() + static_cast<char>(key));
                }
            }

            for (const int& key : alpha) {
                if (inputState.keyDown(key)) {
                    const char character{static_cast<char>(inputState.key(GLFW_KEY_LEFT_SHIFT) ? key : key + 32)};
                    m_text.setText(m_text.text() + character);
                }
            }

            if (inputState.keyDown(GLFW_KEY_BACKSPACE)) {
                m_text.setText(m_text.text().substr(0, m_text.text().length() - 1));
            }

            m_caret.setPosition(bottomRight(m_text));
            m_caret.update(deltaTime, inputState, camera);

            // TODO: Add a `submit` action when the enter key is pressed.

            break;
        }
    }

    void TextField::render(const Camera& camera) const {
        m_shader.bind();
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * camera.viewMatrix());
        m_shader.setUniform("transform", transform());
        m_shader.setUniform("color", glm::vec3{0.9f});

        m_quad.render();

        if (m_state == State::active) {
            // TODO: Put this in configuration struct.
            drawOutline(*this, m_shader, m_quad, glm::vec3{0.0f, 0.5f, 1.0f}, 1.0f, OutlinePlacement::outset);
        }

        m_text.render(camera);
        m_caret.render(camera);
    }

    void TextField::transitionTo(const State state) {
        if (state == m_state) {
            return;
        }

        // User-defined state transition.
        if (m_transitions.contains(state)) {
            m_transitions.at(state)();
        }

        // Default state transitions.
        switch (state) {
        case State::active:
            m_caret.show();
            break;
        case State::inactive:
            m_caret.hide();
            break;
        }

        m_state = state;
    }
} // namespace EconSimPlusPlus
