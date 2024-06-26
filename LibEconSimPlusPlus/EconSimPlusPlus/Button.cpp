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


#include <iostream>

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Button.hpp>
#include <EconSimPlusPlus/Outline.hpp>

namespace EconSimPlusPlus {
    namespace {
        /// Update the text label settings (e.g. position, layer) to match changes to the button.
        /// @param button The button to use as a reference.
        /// @param text The text label to modify.
        void syncSettings(const Button& button, Text& text) {
            const glm::vec2 anchorOffset{calculateAnchorOffset(button.size(), button.anchor())};
            text.setPosition(button.position() + anchorOffset);
            text.setLayer(button.layer());
        }
    } // namespace

    Button::Button(const Text& text, const glm::vec2 position, const Anchor anchor, std::function<void()> callback,
                   const Style& style, const Style& activeStyle, const Style& disabledStyle) :
        m_text(text), m_callback(std::move(callback)), m_normalStyle(style), m_activeStyle(activeStyle),
        m_disabledStyle(disabledStyle), m_currentStyle(style) {
        assert(m_text.anchor() == Anchor::topLeft && "Text anchor within a button must be `topLeft`.");

        setPosition(position);
        Object::setSize(glm::vec2{text.size()});
        setAnchor(anchor);
        setState(State::normal);

        addEventHandler([&](const Event event, const EventData& eventData) { handleEvents(event, eventData.window); });
    }

    void Button::setEnabled(const bool enabled) {
        setState(enabled ? State::normal : State::disabled);
    }

    void Button::setPosition(const glm::vec2 position) {
        Object::setPosition(position);
        syncSettings(*this, m_text);
    }

    void Button::setLayer(const float layer) {
        Object::setLayer(layer);
        syncSettings(*this, m_text);
    }

    void Button::setAnchor(const Anchor anchor) {
        Object::setAnchor(anchor);
        syncSettings(*this, m_text);
    }

    void Button::update(float, const InputState& inputState, const Camera&) {
        switch (m_state) {
        case State::disabled:
        case State::normal:
            break;
        case State::active:
            if (not inputState.mouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
                setState(State::normal);
            }
            break;
        }
    }

    void Button::render(const Camera& camera) const {
        m_shader.bind();

        // Need to add this to camera projection-view matrix otherwise z sorting order will not match other objects.
        const glm::mat4 cameraViewZ = glm::translate(glm::mat4{1.0f}, {0.0f, 0.0f, -camera.position().z});
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * cameraViewZ);

        // Draw the button fill color.
        const glm::vec2 anchorOffset{calculateAnchorOffset(size(), anchor(), size().y)};
        glm::vec2 offsetPosition{position() + anchorOffset};
        glm::mat4 transform{glm::translate(glm::mat4{1.0f}, {offsetPosition, layer()})};
        transform = glm::scale(transform, {size(), 1.0f});
        m_shader.setUniform("transform", transform);
        m_shader.setUniform("color", m_currentStyle.fillColor);
        m_quad.render();

        drawOutline(*this, m_shader, m_quad, m_currentStyle.outline);

        m_text.render(camera);
    }

    void Button::setState(const State state) {
        m_state = state;
        updateStyle();
    }

    void Button::updateStyle() {
        switch (m_state) {
        case State::normal:
            m_currentStyle = m_normalStyle;
            break;
        case State::active:
            m_currentStyle = m_activeStyle;
            break;
        case State::disabled:
            m_currentStyle = m_disabledStyle;
            break;
        }

        m_text.setColor(m_currentStyle.textColor);
    }

    void Button::handleEvents(const Event event, Window& window) {
        switch (event) {
        case Event::mouseEnter:
            if (m_state != State::normal) {
                break;
            }

            window.setCursor(GLFW_HAND_CURSOR);
            break;
        case Event::mouseLeave:
            window.setCursor(GLFW_CURSOR_NORMAL);
            break;
        case Event::mouseClick:
            if (m_state != State::normal) {
                break;
            }

            setState(State::active);
            m_callback();
            break;
        default:
            break;
        }
    }
} // namespace EconSimPlusPlus
