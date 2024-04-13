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
// Created by Anthony Dickson on 13/04/2024.
//

#include "KeyboardState.h"

void KeyboardState::update(const Window &window) {
    std::swap(currentState, previousState);

    for (const auto key: validKeys) {
        currentState[key] = window.getKeyState(key) == GLFW_PRESS;
    }
}

bool KeyboardState::isKeyDown(const int key) const {
    return currentState[key];
}

bool KeyboardState::isKeyUp(const int key) const {
    return !isKeyDown(key);
}
