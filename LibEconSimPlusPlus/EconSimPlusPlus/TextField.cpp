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

// TODO: Add input validation. If the 'submit' action is called on invalid input, the action should be cancelled and the
// user shown a message dialog explaining the problem.
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

        /// Perform the bit-wise and of two bit-wise flags.
        /// @param a The first text field mode bit flag.
        /// @param b The second text field mode bit flag.
        /// @return The bit-wise and of the two flags.
        TextField::Mode operator&(TextField::Mode a, TextField::Mode b) {
            return static_cast<TextField::Mode>(static_cast<int>(a) & static_cast<int>(b));
        }

        /// Check whether a bit-flag contains a target bit-flag.
        /// @param flag A bit-flag.
        /// @param target The bit-flag to test for.
        /// @return Whether the bit-flag contains the target bit-flag.
        bool contains(const TextField::Mode flag, const TextField::Mode target) {
            return (flag & target) == target;
        }

        /// Update a string based on keyboard and mouse input.
        /// @param text The text to modify.
        /// @param input Keyboard and mouse input.
        /// @param config The text field settings.
        /// @return The input text with added or removed characters.
        std::string getTextFromInput(const std::string& text, const InputState& input,
                                     const TextField::Config& config) {
            if (input.key(GLFW_KEY_LEFT_CONTROL) and input.keyDown(GLFW_KEY_BACKSPACE)) {
                return "";
            }

            if (input.keyDown(GLFW_KEY_BACKSPACE)) {
                return text.substr(0, text.length() - 1);
            }

            if (text.length() >= static_cast<std::string::size_type>(config.maxLength)) {
                return text;
            }

            if (contains(config.mode, TextField::Mode::numeric)) {
                for (const int& key : numeric) {
                    if (not input.keyDown(key)) {
                        continue;
                    }

                    return text + static_cast<char>(key);
                }
            }

            if (contains(config.mode, TextField::Mode::alpha)) {
                for (const int& key : alpha) {
                    if (not input.keyDown(key)) {
                        continue;
                    }

                    const char character{static_cast<char>(input.key(GLFW_KEY_LEFT_SHIFT) ? key : key + 32)};
                    return text + character;
                }
            }

            return text;
        }

        /// Set the position of text label relative to a text field.
        /// @note Modifies the text label's position and anchor.
        /// @param textLabel The text to position.
        /// @param textField The text field used as the reference point.
        /// @param padding The horizontal and vertical space between the inner borders of the text field and its child
        /// objects in pixels.
        void setTextPosition(Text& textLabel, const TextField& textField, const glm::vec2 padding) {
            textLabel.setAnchor(Anchor::bottomLeft);
            textLabel.setPosition(bottomLeft(textField) + 0.5f * padding);
        }

    } // namespace

    TextField::TextField(const std::string& placeholder, const Font* font, const Config& config, const Style& style) :
        m_text("", font, style.text), m_placeholder(placeholder, font, style.text), m_config(config), m_style(style),
        m_caret(style.caret) {
        assert(static_cast<int>(placeholder.length()) <= m_config.maxLength &&
               "Placeholder text length is longer than the text field max length.");
        assert(m_config.maxLength > 0 && "A text field's length must be at least one.");

        // '4' and 'M' are chosen because that are roughly the widest characters for numbers and letters, respectively.
        // This ensures that the text field is initialised with a size that should be able to fit the full length
        // string.
        m_text.setText(std::string(m_config.maxLength, m_config.mode == Mode::numeric ? '4' : 'M'));
        Object::setSize(m_text.size() + m_style.padding);
        m_text.setText("");

        setTextPosition(m_text, *this, m_style.padding);
        setTextPosition(m_placeholder, *this, m_style.padding);
        m_placeholder.setColor(m_style.placeholderTextColor);

        m_caret.setSize(glm::vec2{m_caret.size().x, m_text.size().y});
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

    std::string TextField::text() const {
        return m_text.text();
    }

    void TextField::setText(const std::string& text) {
        m_text.setText(text);
    }

    void TextField::setTransition(const State state, const std::function<void()>& function) {
        m_transitions[state] = function;
    }

    void TextField::setPosition(const glm::vec2 position) {
        Object::setPosition(position);
        setTextPosition(m_text, *this, m_style.padding);
        setTextPosition(m_placeholder, *this, m_style.padding);
        m_caret.setPosition(bottomRight(m_text));
    }

    void TextField::setLayer(const float layer) {
        Object::setLayer(layer);
        m_text.setLayer(layer);
        m_placeholder.setLayer(layer);
        m_caret.setLayer(layer);
    }

    void TextField::update(const float deltaTime, const InputState& inputState, const Camera& camera) {
        switch (m_state) {
        case State::inactive:
            break;
        case State::active:
            if (inputState.keyDown(GLFW_KEY_ESCAPE) or
                (inputState.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and
                 not contains(*this, screenToWorldCoordinates(inputState.mousePosition(), camera)))) {
                transitionTo(State::inactive);
                break;
            }

            m_text.setText(getTextFromInput(m_text.text(), inputState, m_config));
            m_caret.setPosition(bottomRight(m_text));
            m_caret.update(deltaTime, inputState, camera);

            // TODO: Add a `submit` action when the enter key is pressed.

            break;
        }
    }

    void TextField::render(const Camera& camera) const {
        const glm::mat4 transform{glm::scale(glm::translate(glm::mat4{1.0f}, glm::vec3{bottomLeft(*this), layer()}),
                                             glm::vec3{size(), 1.0f})};

        m_shader.bind();
        m_shader.setUniform("projectionViewMatrix", projectionViewMatrix(camera));
        m_shader.setUniform("transform", transform);
        m_shader.setUniform("color", m_style.fillColor);

        m_quad.render();

        if (m_state == State::active) {
            Outline::draw(*this, m_shader, m_quad, m_style.outline);
        }

        m_text.text().empty() ? m_placeholder.render(camera) : m_text.render(camera);
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
