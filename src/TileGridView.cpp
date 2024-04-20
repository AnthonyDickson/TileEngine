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

#include "TileGridView.h"

#include <algorithm>
#include <iostream>

#include "glm/ext/matrix_transform.hpp"


TileGridView::TileGridView(std::shared_ptr<const TileGrid> tileGrid_, const Size<int>& viewport_, const int tileSize_)
: tileGrid(std::move(tileGrid_)), viewport(viewport_), tileSize(tileSize_) {
    const std::vector vertexData{
        // X, Y, U, V (2D coordinates, Texture coordinates).
        // [0]    -> [1,4]
        //         /
        //       /
        // [2, 5] -> [3]
        0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
    };

    vao.bind();
    vbo.loadData(vertexData, std::vector{2, 2});
}

void TileGridView::updateViewport(const Size<int> windowSize) {
    const int tilesPerHeight = windowSize.height / tileSize;
    const int tilesPerWidth = windowSize.width / tileSize;

    viewport.width = std::clamp(tilesPerWidth, 1, tileGrid->width);
    viewport.height = std::clamp(tilesPerHeight, 1, tileGrid->height);

    setRowOffset(rowOffset);
    setColOffset(colOffset);
}

void TileGridView::processInput(const KeyboardState& keyboardState) {
    if (keyboardState.isKeyDown(GLFW_KEY_W)) {
        setRowOffset(rowOffset - 1);
    }
    else if (keyboardState.isKeyDown(GLFW_KEY_S)) {
        setRowOffset(rowOffset + 1);
    }

    if (keyboardState.isKeyDown(GLFW_KEY_A)) {
        setColOffset(colOffset - 1);
    }
    else if (keyboardState.isKeyDown(GLFW_KEY_D)) {
        setColOffset(colOffset + 1);
    }
}

void TileGridView::setRowOffset(const int value) {
    rowOffset = std::clamp(value, 0, std::max(0, tileGrid->height - viewport.height));
}

void TileGridView::setColOffset(const int value) {
    colOffset = std::clamp(value, 0, std::max(0, tileGrid->width - viewport.width));
}

void TileGridView::render(const glm::mat4 &projectionViewMatrix) const {
    constexpr glm::mat4 identity{1.0f};

    shader.bind();
    shader.setUniform("projectionViewMatrix", projectionViewMatrix);
    vao.bind();

    for (int row = 0; row < viewport.height; ++row) {
        for (int col = 0; col < viewport.width; ++col) {
            glm::mat4 model = translate(
                identity,
                glm::vec3{static_cast<float>(col * tileSize), static_cast<float>(row * tileSize), 0.0}
            );
            model = scale(model, glm::vec3{tileSize, tileSize, 0.0});

            shader.setUniform("model", model);
            tileGrid->bindTextureAt(row + rowOffset, col + colOffset);
            vbo.drawArrays();
        }
    }
}
