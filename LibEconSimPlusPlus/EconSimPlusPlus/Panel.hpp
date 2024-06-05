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
// Created by Anthony Dickson on 04/06/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_PANEL_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_PANEL_HPP

#include "glm/vec2.hpp"

#include <EconSimPlusPlus/GuiObject.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Shader.hpp>

// TODO: Allow objects to be added to the panel.
// TODO: Make the panel's object transforms to be relative to the panel's transform.
// TODO: Ensure that objects are not rendered past the borders of the panel.
namespace EconSimPlusPlus {

    /// Provides a logical and visual container for other objects.
    class Panel final : public GUIObject {
    public:
        Panel(glm::vec2 position, glm::vec2 size, Anchor anchor);

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Camera& camera) const override;

    private:
        Quad m_quad{};
        Shader m_shader{Shader::create("resource/shader/grid.vert", "resource/shader/grid.frag")};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_PANEL_HPP
