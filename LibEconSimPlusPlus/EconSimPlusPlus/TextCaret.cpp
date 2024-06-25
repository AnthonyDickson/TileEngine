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
// Created by Anthony Dickson on 25/06/2024.
//

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/TextCaret.hpp>

namespace EconSimPlusPlus {
    void TextCaret::hide() {
        m_state = State::hidden;
    }

    void TextCaret::show() {
        m_state = State::visible;
        m_time = 0.0f;
    }

    void TextCaret::update(const float deltaTime, const InputState&, const Camera&) {
        m_time += deltaTime;
    }

    void TextCaret::render(const Camera& camera) const {
        if (m_state == State::hidden) {
            return;
        }

        m_shader.bind();
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * camera.viewMatrix());
        m_shader.setUniform(
            "transform",
            glm::scale(glm::translate(glm::mat4{1.0f}, glm::vec3{position(), layer()}), glm::vec3{size(), 1.0f}));
        const float alpha{0.5f * (sin(m_time) + 1.0f)};
        m_shader.setUniform("color", glm::vec4{0.0f, 0.0f, 0.0f, alpha});
        m_quad.render();
    }
} // namespace EconSimPlusPlus
