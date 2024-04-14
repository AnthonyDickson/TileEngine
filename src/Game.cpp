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

#include <utility>

#include "glm/ext/matrix_transform.hpp"

#include "Game.h"
#include "Shader.h"
#include "Size.h"
#include "TileRegistry.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

bool Game::isInitialised = false;

Game::Game(std::unique_ptr<Window> window_, std::shared_ptr<TileGrid> tileGrid_, TileGridView tileGridView_)
: window(std::move(window_)), tileGrid(std::move(tileGrid_)), tileGridView(std::move(tileGridView_)) {
    assert(!isInitialised && "Can only have one instance of `Game`.");
    isInitialised = true;
}

Game Game::create(Size<int> windowSize, Size<int> tileGridSize, int tileSize) {
    auto window{std::make_unique<Window>(windowSize.width, windowSize.height, "EconSimPlusPlus")};

    const auto tileGrid{std::make_shared<TileGrid>(tileGridSize.width, tileGridSize.height)};

    const int tilesPerHeight = windowSize.height / tileSize;
    const int tilesPerWidth = windowSize.width / tileSize;
    TileGridView tileGridView{tileGrid, {tilesPerWidth, tilesPerHeight}};

    return {(std::move(window)), tileGrid, tileGridView};
}

void Game::update(float deltatime) {
    keyboardState.update(window);
    tileGridView.processInput(keyboardState);
}

void Game::run() {
    Camera camera{
        {static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight())},
        {0.0f, 0.0f, 3.0f}
    };

    // Create the vertex array object.
    const VertexArray vao{};
    vao.bind();

    constexpr int tilesPerHeight{32};
    int tileSize{window->getHeight() / tilesPerHeight};
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
    tileGrid->at(0, 0) = 1;
    tileGrid->at(15, 15) = 1;
    tileGrid->at(31, 31) = 1;
    tileGrid->at(47, 47) = 1;
    tileGrid->at(63, 63) = 1;

    auto update_fn = [&](const float deltaTime) {
        if (window->getKeyState(GLFW_KEY_ESCAPE)) {
            window->close();
            return;
        }

        if (window->hasWindowSizeChanged()) {
            camera.onWindowResize({
                static_cast<float>(window->getWidth()),
                static_cast<float>(window->getHeight())
            });
            tileSize = window->getHeight() / tilesPerHeight;
            // TODO: Increase tile view size to fill new screen size.
        }

        update(deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        const glm::mat4 projectionViewMatrix = camera.getPerspectiveMatrix() * camera.getViewMatrix();

        shader.bind();
        shader.setUniform("projectionViewMatrix", projectionViewMatrix);
        vao.bind();

        for (const auto& [transform, tileID]: tileGridView.getTilePositionAndIds(static_cast<float>(tileSize))) {
            shader.setUniform("model", transform);
            tileRegistry[tileID].bind();
            vbo.drawArrays();
        }
    };

    // auto update_fn = [this](float deltaTime) { update(deltaTime); };
    window->runMainLoop(update_fn);
}
