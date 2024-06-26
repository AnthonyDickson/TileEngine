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

// ReSharper disable once CppUnusedIncludeDirective
#include <memory>

#include "glm/vec2.hpp"

#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/Outline.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Shader.hpp>

namespace EconSimPlusPlus {

    /// Provides a logical and visual container for other objects.
    class Panel final : public Object {
    public:
        /// Configuration for panel appearance.
        struct Style {
            /// The color to fill the background of the panel with.
            glm::vec3 fillColor{1.0f};
            /// The configuration for the appearance of the outline.
            OutlineStyle outline{
                .color = glm::vec3{0.0f},
                .thickness = 1.0f,
                .placement = OutlinePlacement::inset
            };
        };

        /// Create an empty panel.
        /// @param position The x and y coordinates of the panel in world space.
        /// @param style Configuration for panel apperance.
        Panel(glm::vec2 position, const Style& style);

        /// Add an object to the panel.
        /// @param object An object.
        void addObject(std::unique_ptr<Object> object);

        void setPosition(glm::vec2 position) override;
        void setSize(glm::vec2 size) override;
        void setLayer(float layer) override;

        void notify(Event event, EventData eventData) override;
        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Camera& camera) const override;

    private:
        /// Recalculate the panel objects layout from scratch.
        /// @note Intended to be called after the panel's position or size are updated so that objects are positioned
        /// correctly.
        void recalculateObjectPositions();

        /// Configuration for panel apperance.
        Style m_style;
        /// The quad geometry for drawing the panel background and outlines.
        Quad m_quad{};
        /// The shader for coloring the quad with a solid color.
        Shader m_shader{Shader::create("resource/shader/grid.vert", "resource/shader/grid.frag")};
        /// Objects contained by this panel.
        std::vector<std::unique_ptr<Object>> m_objects{};
        /// Where to place the next object.
        glm::vec2 m_nextObjectPosition{};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_PANEL_HPP
