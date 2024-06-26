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
// Created by Anthony Dickson on 26/06/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTFIELDSTYLE_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTFIELDSTYLE_HPP

#include <EconSimPlusPlus/Outline.hpp>
#include <EconSimPlusPlus/TextCaretStyle.hpp>

namespace EconSimPlusPlus {

    /// The configuration for the appearance of a text field.
    struct TextFieldStyle {
        /// The color of the text inside the text field.
        glm::vec3 textColor{0.0f};
        /// The color with which to fill the text field background.
        glm::vec3 fillColor{1.0f};
        /// Padding around internal GUI objects in pixels.
        glm::vec2 padding{8.0f, 8.0f};
        /// The style of the blinking text cursor.
        TextCaretStyle caret{.width = 2.0f, .color = glm::vec3{0.0f}};
        /// The configuration for the appearance of the outline.
        OutlineStyle outline{
            .color = glm::vec3{0.0f, 0.5f, 1.0f}, .thickness = 1.0f, .placement = OutlinePlacement::outset};
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTFIELDSTYLE_HPP
