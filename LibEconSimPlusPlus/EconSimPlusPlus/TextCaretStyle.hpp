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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTCARETSTYLE_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTCARETSTYLE_HPP

#include "glm/vec3.hpp"

namespace EconSimPlusPlus {

    /// The configuration for the appearance of a text cursor.
    struct TextCaretStyle {
        /// The color of the blinking text cursor.
        glm::vec3 color{0.0f};
        /// The width of the blinking text cursor in pixels.
        float width{2.0f};
    };

} // EconSimPlusPlus

#endif //LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TEXTCARETSTYLE_HPP
