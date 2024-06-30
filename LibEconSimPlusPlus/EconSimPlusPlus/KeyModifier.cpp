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
// Created by Anthony Dickson on 30/06/2024.
//

#include <stdexcept>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <EconSimPlusPlus/KeyModifier.hpp>

namespace EconSimPlusPlus::KeyModifier {
    KeyModifier operator|(const KeyModifier a, const KeyModifier b) {
        return static_cast<KeyModifier>(static_cast<int>(a) | static_cast<int>(b));
    }

    KeyModifier operator|=(KeyModifier& a, const KeyModifier b) {
        a = a | b;
        return a;
    }

    KeyModifier operator&(const KeyModifier a, const KeyModifier b) {
        return static_cast<KeyModifier>(static_cast<int>(a) & static_cast<int>(b));
    }

    bool contains(const KeyModifier flags, const KeyModifier target) {
        return (flags & target) == target;
    }

    bool contains(const KeyModifier flags, const int target) {
        switch (target) {
        case GLFW_MOD_CONTROL:
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
            return contains(flags, control);
        case GLFW_MOD_SHIFT:
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            return contains(flags, shift);
        case GLFW_MOD_ALT:
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:
            return contains(flags, alt);
        case GLFW_MOD_SUPER:
        case GLFW_KEY_LEFT_SUPER:
        case GLFW_KEY_RIGHT_SUPER:
            return contains(flags, super);
        default:
            throw std::invalid_argument("The given key code is not a key modifier (control, shift, alt or super).");
        }
    }
} // namespace EconSimPlusPlus::KeyModifier
