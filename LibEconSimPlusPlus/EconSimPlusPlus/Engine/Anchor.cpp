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

#include <EconSimPlusPlus/Engine/Anchor.hpp>

namespace EconSimPlusPlus::Engine {
    glm::vec2 calculateAnchorOffset(const glm::vec2 size, const Anchor anchor) {
        switch (anchor) {
        case Anchor::bottomLeft:
            return glm::vec2{0.0f, size.y};
        case Anchor::bottomRight:
            return glm::vec2{-size.x, size.y};
        case Anchor::topLeft:
            return glm::vec2{0.0f, 0.0f};
        case Anchor::topRight:
            return glm::vec2{-size.x, 0.0f};
        case Anchor::center:
            return {-size.x / 2.0f, size.y / 2.0f};
        default:
            return glm::vec2{0.0f};
        }
    }
} // namespace EconSimPlusPlus::Engine
