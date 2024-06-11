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
// Created by Anthony Dickson on 29/05/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_BUTTONSTYLE_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_BUTTONSTYLE_HPP

#include "glm/vec3.hpp"

namespace EconSimPlusPlus {
    /// Configuration for button appearance.
    struct ButtonStyle {
        /// The color of the button's text.
        glm::vec3 textColor{0.0f};
        /// The color to fill the background of the button with.
        glm::vec3 fillColor{1.0f};
        /// The color to use for the border.
        glm::vec3 outlineColor{0.0f, 0.0f, 0.0f};
        /// The thickness of the border in pixels.
        float outlineThickness{0.0f};
    };
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_BUTTONSTYLE_HPP
