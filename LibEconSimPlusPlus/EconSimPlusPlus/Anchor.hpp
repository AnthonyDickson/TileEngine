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
// Created by Anthony Dickson on 19/05/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ANCHOR_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ANCHOR_HPP

#include "glm/vec2.hpp"

namespace EconSimPlusPlus {
    /// The point on an object that its position refers to.
    enum class Anchor { bottomLeft, bottomRight, topLeft, topRight, center };

    /// Calculate the positional offset for the given object size and anchor that gives the top left corner of an object
    /// when added to its raw position.
    /// @param size The dimensions (width, height) of the object.
    /// @param anchor The point on the object to use as the origin.
    /// @param verticalBaseline A vertical offset to ensure that the correct point on the y-axis is used as the origin.
    /// Important if the object is using world coordinates where +y points up.
    /// @return A 2D offset that, when added to the raw position, gives the top left corner of an object.
    /// @note Assumes screen space coordinates, i.e., +y points down.
    [[nodiscard]] glm::vec2 calculateAnchorOffset(glm::vec2 size, Anchor anchor, float verticalBaseline = 0.0f);
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ANCHOR_HPP
