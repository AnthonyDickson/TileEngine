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

#include "TextField.hpp"

namespace EconSimPlusPlus {
    TextField::TextField() {
        Object::setSize({32.0f, 32.0f});
        Object::setScale({32.0f, 32.0f});
        Object::setLayer(99.0f);
    }
    void TextField::update(float, const InputState&, const Camera&) {
    }

    void TextField::render(const Camera& camera) const {
        m_shader.bind();
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * camera.viewMatrix());
        m_shader.setUniform("transform", transform());
        m_shader.setUniform("color", glm::vec3{1.0f, 0.0f, 1.0f});

        m_quad.render(GL_TRIANGLE_STRIP);
    }
} // namespace EconSimPlusPlus
