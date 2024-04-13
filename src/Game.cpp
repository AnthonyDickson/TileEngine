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

#include "Game.h"

#include <Shader.h>
#include <TileGrid.h>
#include <TileRegistry.h>
#include <VertexArray.h>
#include <VertexBuffer.h>

#include "glm/ext/matrix_transform.hpp"

class Texture;
bool Game::isInitialised = false;

Game::Game(const Window& window_) : window(window_) {
    assert(!isInitialised && "Can only have one instance of `Game`.");
    isInitialised = true;
}


void Game::update(float deltatime) {
    keyboardState.update(window);
}

void Game::run() {
    Camera camera{{static_cast<float>(window.getWidth()), static_cast<float>(window.getHeight())},
                  {0.0f,        0.0f, 3.0f}};

    // Create the vertex array object.
    const VertexArray vao{};
    vao.bind();

    constexpr int tilesPerHeight{32};
    int tileSize{window.getHeight() / tilesPerHeight};
    int tilesPerWidth{window.getWidth() / tileSize};
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
    const VertexBuffer vbo{vertexData, 4, std::vector{2, 2}};

    const Shader shader{"resource/shader/tile.vert", "resource/shader/tile.frag"};

    auto container{std::make_shared<const Texture>("resource/container2.png", GL_TEXTURE0)};
    auto awesomeFace{std::make_shared<const Texture>("resource/awesomeface.png", GL_TEXTURE0)};

    TileRegistry tileRegistry{};
    tileRegistry.emplace(container);
    tileRegistry.emplace(awesomeFace);
    TileGrid tileGrid{64, 64};
    // ReSharper disable once CppExpressionWithoutSideEffects
    tileGrid[0, 0] = 1;
    // ReSharper disable once CppExpressionWithoutSideEffects
    tileGrid[15, 15] = 1;
    // ReSharper disable once CppExpressionWithoutSideEffects
    tileGrid[31, 31] = 1;
    // ReSharper disable once CppExpressionWithoutSideEffects
    tileGrid[47, 47] = 1;
    // ReSharper disable once CppExpressionWithoutSideEffects
    tileGrid[63, 63] = 1;

    int colOffset{0};
    int rowOffset{0};

    auto update_fn = [&](const float deltaTime) {
        if (window.getKeyState(GLFW_KEY_ESCAPE)) {
            window.close();
            return;
        }

        if (window.hasWindowSizeChanged()) {
            camera.onWindowResize({static_cast<float>(window.getWidth()),
                                   static_cast<float>(window.getHeight())});
            tileSize = window.getHeight() / tilesPerHeight;
            tilesPerWidth = window.getWidth() / tileSize;
        }

        update(deltaTime);

        if (window.getKeyState(GLFW_KEY_W) == GLFW_PRESS) {
            rowOffset = std::max(rowOffset - 1, 0);
        } else if (window.getKeyState(GLFW_KEY_S) == GLFW_PRESS) {
            rowOffset = std::min(rowOffset + 1, tileGrid.height - tilesPerHeight);
        }

        if (window.getKeyState(GLFW_KEY_A) == GLFW_PRESS) {
            colOffset = std::max(colOffset - 1, 0);
        } else if (window.getKeyState(GLFW_KEY_D) == GLFW_PRESS) {
            colOffset = std::min(colOffset + 1, tileGrid.width - tilesPerWidth);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        const glm::mat4 projectionViewMatrix = camera.getPerspectiveMatrix() * camera.getViewMatrix();

        shader.bind();
        shader.setUniform("projectionViewMatrix", projectionViewMatrix);
        vao.bind();

        for (int row = 0; row < tilesPerHeight; ++row) {
            for (int col = 0; col < tilesPerWidth; ++col) {
                glm::mat4 model{1.0f};
                model = translate(
                        model,
                        glm::vec3{static_cast<float>(col * tileSize), static_cast<float>(row * tileSize), 0.0}
                );
                model = scale(model, glm::vec3{static_cast<float>(tileSize), static_cast<float>(tileSize), 0.0});

                shader.setUniform("model", model);
                // ReSharper disable once CppExpressionWithoutSideEffects
                const auto tileID = tileGrid[row + rowOffset, col + colOffset];
                tileRegistry[tileID].bind();
                vbo.drawArrays();
            }
        }
    };

    // auto update_fn = [this](float deltaTime) { update(deltaTime); };
    window.runMainLoop(update_fn);
}
