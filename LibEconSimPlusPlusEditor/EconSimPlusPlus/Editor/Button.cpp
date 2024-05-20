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

#include <EconSimPlusPlus/Editor/Button.hpp>

namespace EconSimPlusPlus::Editor {
    Button::Button(const Engine::Font* font, std::string text, const glm::vec2 position,
                   std::function<void()> callback) :
        m_text(std::move(text)), m_callback(std::move(callback)), m_font(font) {
        // TODO: This seems to calculate an innacurate size (too tall and wide). Make sure it produces a tight bounding
        // box. Seems to be out by a factor of two.
        const glm::vec2 textSize{font->calculateTextSize(m_text) / 2.0f};
        // TODO: Create TextLabel class that stores text render settings, position and dimensions.
        // TODO: Calculate button size w/ padding.
        setPosition(position);
        setSize(textSize);
        setAnchor(Engine::Anchor::topLeft);

        // Separate vao/vbo for outline.
        m_vao.bind();
        m_vbo.loadData({0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f}, {2});
    }

    void Button::update(float, const Engine::InputState& inputState, const Engine::Camera& camera) {
        if (inputState.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            glm::vec2 cursorPos{Engine::screenToWorldCoordinates(inputState.getMousePosition(), camera)};

            if (contains(cursorPos)) {
                m_callback();
            }
        }
    }

    void Button::render(const Engine::Camera& camera) const {
        // TODO: Make these options configurable.
        constexpr glm::vec3 outlineColor{1.0f, 0.0f, 1.0f};
        constexpr glm::vec3 fillColor{1.0f};

        m_shader.bind();

        // Need to add this to camera projection-view matrix otherwise z sorting order will not match other objects.
        const glm::mat4 cameraViewZ = glm::translate(glm::mat4{1.0f}, {0.0f, 0.0f, -camera.position().z});
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * cameraViewZ);

        const glm::vec2 anchorOffset{Engine::calculateAnchorOffset(size(), anchor(), size().y)};
        glm::mat4 transform{glm::translate(glm::mat4{1.0f}, {position() + anchorOffset, layer()})};
        transform = glm::scale(transform, {size(), 1.0f});
        m_shader.setUniform("transform", transform);

        m_vao.bind();
        m_shader.setUniform("color", fillColor);
        m_vbo.drawArrays(GL_TRIANGLE_STRIP);
        m_shader.setUniform("color", outlineColor);
        m_vbo.drawArrays(GL_LINES);

        // TODO: Add padding for text.
        m_font->render(m_text, {position(), layer()}, camera, {.anchor = anchor(), .color = {0.0f, 0.0f, 0.0f}});
    }
} // namespace EconSimPlusPlus::Editor
