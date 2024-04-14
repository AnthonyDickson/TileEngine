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
// Created by Anthony Dickson on 14/04/2024.
//

#include "TileGridViewer.h"

#include <utility>

TileGridView::TileGridView(std::shared_ptr<const TileGrid> tileGrid_, const Size<int>& viewport_): tileGrid(std::move(tileGrid_)), viewport(viewport_) {}

void TileGridView::processInput(const KeyboardState& keyboardState) {
    if (keyboardState.isKeyDown(GLFW_KEY_W)) {
        rowOffset = std::max(rowOffset - 1, 0);
    }
    else if (keyboardState.isKeyDown(GLFW_KEY_S)) {
        rowOffset = std::min(rowOffset + 1, tileGrid->height - viewport.height);
    }

    if (keyboardState.isKeyDown(GLFW_KEY_A)) {
        colOffset = std::max(colOffset - 1, 0);
    }
    else if (keyboardState.isKeyDown(GLFW_KEY_D)) {
        colOffset = std::min(colOffset + 1, tileGrid->width - viewport.width);
    }
}

std::vector<MatrixTileIDPair> TileGridView::getTilePositionAndIds(const float tileSize) const {
    std::vector<MatrixTileIDPair> result{};
    result.reserve(viewport.height * viewport.width);

    constexpr glm::mat4 identity{1.0f};

    for (int row = 0; row < viewport.height; ++row) {
        for (int col = 0; col < viewport.width; ++col) {
            glm::mat4 model = translate(
                identity,
                glm::vec3{static_cast<float>(col) * tileSize, static_cast<float>(row) * tileSize, 0.0}
            );
            model = scale(model, glm::vec3{tileSize, tileSize, 0.0});

            result.push_back({model, tileGrid->get(row + rowOffset, col + colOffset)});
        }
    }

    return result;
}
