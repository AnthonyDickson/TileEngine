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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_KEYMODS_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_KEYMODS_HPP

namespace EconSimPlusPlus::KeyModifier {
    /// Bit-flag for keyboard key modifiers.
    enum KeyModifier { none = 1 << 0, control = 1 << 1, shift = 1 << 2, alt = 1 << 3, super = 1 << 4 };

    /// Bit-wise or of two key modifier bit-flags.
    KeyModifier operator|(KeyModifier a, KeyModifier b);

    /// Bit-wise or setter of two key modifier bit-flags.
    KeyModifier operator|=(KeyModifier& a, KeyModifier b);

    /// Bit-wise and of two key modifier bit-flags.
    KeyModifier operator&(KeyModifier a, KeyModifier b);

    /// Check whether the flags contain a given enum value.
    /// @param flags The source key modifier bit-flags to test against. This can be a combination of enum values.
    /// @param target The key modifier to check for.
    /// @return Whether the flags contain the target enum value.
    bool contains(KeyModifier flags, KeyModifier target);

    /// Check whether the flags contain a given GLFW key modifier code.
    /// @param flags The source key modifier bit-flags to test against. This can be a combination of enum values.
    /// @param target The GLFW key modifier code to check for.
    /// @return Whether the flags contain the GLFW key modifier code
    /// @note Examples of valid values: GLFW_MOD_CONTROL, GLFW_MOD_SHIFT, GLFW_MOD_ALT, GLFW_MOD_SUPER.
    bool contains(KeyModifier flags, int target);
} // namespace EconSimPlusPlus::KeyModifier


#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_KEYMODS_HPP
