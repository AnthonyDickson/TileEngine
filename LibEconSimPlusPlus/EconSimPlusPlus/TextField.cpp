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
        /// @param keyCode The code of the key that was pressed.
        /// @param keyModifiers Which key modifiers (control, shift, alt, super) are pressed.
        /// @param config The text field settings.
        /// @return The input text with added or removed characters.
        std::string getTextFromInput(const std::string& text, const int keyCode,
                                     const KeyModifier::KeyModifier keyModifiers, const TextField::Config& config) {
            if (keyCode == GLFW_KEY_BACKSPACE and contains(keyModifiers, GLFW_MOD_CONTROL)) {
                return "";
            }

            if (keyCode == GLFW_KEY_BACKSPACE) {
                return text.substr(0, text.length() - 1);
            }

            if (text.length() >= static_cast<std::string::size_type>(config.maxLength)) {
                return text;
            }

            if (contains(config.mode, TextField::Mode::numeric) and GLFW_KEY_0 <= keyCode and keyCode <= GLFW_KEY_9) {
                return text + static_cast<char>(keyCode);
            }

            if (contains(config.mode, TextField::Mode::alpha) and GLFW_KEY_A <= keyCode and keyCode <= GLFW_KEY_Z) {
                const char character{
                    static_cast<char>(contains(keyModifiers, GLFW_MOD_SHIFT) ? keyCode : keyCode + 32)};
                return text + character;
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

        m_caret.setHeight(m_text.size().y);
        m_caret.hide();

        addEventHandler([&](const Event event, const EventData& eventData) {
            switch (event) {
            case Event::mouseEnter:
                eventData.window.setCursor(GLFW_IBEAM_CURSOR);
                break;
            case Event::mouseLeave:
                eventData.window.setCursor(GLFW_CURSOR_NORMAL);
                break;
            case Event::focus:
                transitionTo(State::active);
                break;
            case Event::defocus:
                transitionTo(State::inactive);
                break;
            case Event::keyDown:
                assert(
                    eventData.keyCode.has_value() && eventData.keyModifiers.has_value() &&
                    "Received keyDown event but either the key code or key modifiers were not set in the event data.");

                switch (*eventData.keyCode) {
                case GLFW_KEY_ENTER:
                    if (m_inputValidator and not m_inputValidator(text())) {
                        setText(m_rollbackString);
                        break;
                    }

                    // The text is set to the rollback text when changing to the inactive state. Setting the value
                    // here ensures that the submitted value is not discarded.
                    m_rollbackString = text();

                    if (m_submitAction) {
                        m_submitAction(text());
                    }

                    transitionTo(State::inactive);
                    break;
                default:
                    setText(getTextFromInput(text(), *eventData.keyCode, *eventData.keyModifiers, m_config));
                    break;
                }
            default:
                break;
            }
        });
    }

    void TextField::setInputValidator(const InputValidator& function) {
        m_inputValidator = function;
    }

    void TextField::setSubmitAction(const SubmitAction& function) {
        m_submitAction = function;
    }

    std::string TextField::text() const {
        return m_text.text();
    }

    void TextField::setText(const std::string& text) {
        m_text.setText(text);
        m_caret.setPosition(bottomRight(m_text));
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
            m_caret.update(deltaTime, inputState, camera);
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

        text().empty() ? m_placeholder.render(camera) : m_text.render(camera);
        m_caret.render(camera);
    }

    void TextField::transitionTo(const State state) {
        if (state == m_state) {
            return;
        }

        m_state = state;

        switch (m_state) {
        case State::active:
            m_caret.show();
            m_rollbackString = text();
            break;
        case State::inactive:
            m_caret.hide();
            setText(m_rollbackString);
            break;
        }
    }
} // namespace EconSimPlusPlus
