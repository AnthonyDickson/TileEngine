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
// Created by Anthony Dickson on 27/05/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_FONTSTYLE_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_FONTSTYLE_HPP

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include <EconSimPlusPlus/Anchor.hpp>

namespace EconSimPlusPlus {
    /// The collection of settings for drawing text on screen.
    struct FontStyle {
        /// The colour to render the text.
        glm::vec3 color{1.0f};
        /// The height, in pixels, at which to draw the text. Note that the width will be scaled proportionately to
        /// maintain the aspect ratio.
        float size{32.0f};
        /// The amount of horizontal and vertical space to add around the text measured in pixels.
        glm::vec2 padding{0.0f};
        /// The point on the text that the position refers to.
        Anchor anchor{Anchor::topLeft};
        /// The value between zero and one in the font SDF that indicates an edge.
        /// Increasing this value will shrink the font and increasing it will expand the font.
        float sdfThreshold{0.5f};
        /// The amount to smooth the edges as a ratio of the SDF range.
        float edgeSmoothness{0.01f};
        /// The size of the outline to draw around the edge of the text as a ratio of the SDF range. Set to zero to
        /// disable outlines.
        float outlineSize{0.0f};
        /// The colour of the text outline.
        glm::vec3 outlineColor{0.0f};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_FONTSTYLE_HPP
