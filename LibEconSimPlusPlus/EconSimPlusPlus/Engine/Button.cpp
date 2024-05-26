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


#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Engine/Button.hpp>

namespace EconSimPlusPlus::Engine {
    Button::Button(const Text& text, const glm::vec2 position, std::function<void()> callback) :
        m_text(text), m_callback(std::move(callback)) {
        setPosition(position);
        const glm::vec2 textSize{text.size()};
        setSize(textSize);
        setAnchor(Anchor::topLeft);

        // Separate vao/vbo for outline.
        m_vao.bind();
        m_vbo.loadData({0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f}, {2});
    }

    void Button::setPosition(const glm::vec2 position) {
        GUIObject::setPosition(position);
        m_text.setPosition(position);
    }

    void Button::setLayer(const float layer) {
        GUIObject::setLayer(layer);
        m_text.setLayer(layer);
    }

    void Button::update(float, const InputState& inputState, const Camera& camera) {
        if (inputState.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            // ReSharper disable once CppTooWideScopeInitStatement
            const glm::vec2 cursorPos{screenToWorldCoordinates(inputState.getMousePosition(), camera)};

            if (contains(cursorPos)) {
                m_callback();
            }
        }
    }

    void Button::render(const Camera& camera) const {
        // TODO: Make these options configurable.
        constexpr glm::vec3 outlineColor{1.0f, 0.0f, 1.0f};
        constexpr glm::vec3 fillColor{1.0f};

        m_shader.bind();

        // Need to add this to camera projection-view matrix otherwise z sorting order will not match other objects.
        const glm::mat4 cameraViewZ = glm::translate(glm::mat4{1.0f}, {0.0f, 0.0f, -camera.position().z});
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * cameraViewZ);

        const glm::vec2 anchorOffset{calculateAnchorOffset(size(), anchor(), size().y)};
        glm::mat4 transform{glm::translate(glm::mat4{1.0f}, {position() + anchorOffset, layer()})};
        transform = glm::scale(transform, {size(), 1.0f});
        m_shader.setUniform("transform", transform);

        m_vao.bind();
        m_shader.setUniform("color", fillColor);
        m_vbo.drawArrays(GL_TRIANGLE_STRIP);
        m_shader.setUniform("color", outlineColor);
        m_vbo.drawArrays(GL_LINES);

        m_text.render(camera);
    }
} // namespace EconSimPlusPlus::Engine
