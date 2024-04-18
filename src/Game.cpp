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

#include <iostream>
#include <utility>

#include "glm/ext/matrix_transform.hpp"

#include "Game.h"
#include "Size.h"
#include "TileRegistry.h"

bool Game::isInitialised = false;

Game::Game(std::unique_ptr<Window> window_, std::shared_ptr<TileGrid> tileGrid_, std::shared_ptr<TileGridView> tileGridView_):
    window(std::move(window_)), tileGrid(std::move(tileGrid_)), tileGridView(std::move(tileGridView_)),
    camera{{static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight())}, {0.0f, 0.0f, 3.0f}} {
    assert(!isInitialised && "Can only have one instance of `Game`.");
    isInitialised = true;
}

Game Game::create(Size<int> windowSize, Size<int> tileGridSize, const int tileSize) {
    auto window{std::make_unique<Window>(windowSize.width, windowSize.height, "EconSimPlusPlus")};

    const auto tileGrid{std::make_shared<TileGrid>(tileGridSize.width, tileGridSize.height)};

    const int tilesPerHeight = windowSize.height / tileSize;
    const int tilesPerWidth = windowSize.width / tileSize;
    // ReSharper disable once CppTemplateArgumentsCanBeDeduced
    const Size<int> viewport{tilesPerWidth, tilesPerHeight};
    const auto tileGridView{std::make_shared<TileGridView>(tileGrid, viewport, tileSize)};

    return {std::move(window), tileGrid, tileGridView};
}

void Game::update() {
    if (window->hasWindowSizeChanged()) {
        camera.onWindowResize({
            static_cast<float>(window->getWidth()),
            static_cast<float>(window->getHeight())
        });
        tileGridView->updateViewport(window->getSize());
    }

    keyboardState.update(window);
    tileGridView->processInput(keyboardState);
}

void Game::render() const {
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    const glm::mat4 projectionViewMatrix = camera.getPerspectiveMatrix() * camera.getViewMatrix();

    tileGridView->render(projectionViewMatrix, tileRegistry);
}

void Game::run() {
    auto container{std::make_shared<const Texture>("resource/container2.png", GL_TEXTURE0)};
    auto awesomeFace{std::make_shared<const Texture>("resource/awesomeface.png", GL_TEXTURE0)};

    tileRegistry.emplace(container);
    tileRegistry.emplace(awesomeFace);
    tileGrid->at(0, 0) = 1;
    tileGrid->at(15, 15) = 1;
    tileGrid->at(31, 31) = 1;
    tileGrid->at(47, 47) = 1;
    tileGrid->at(63, 63) = 1;

    auto lastFrameTime{static_cast<float>(glfwGetTime())};

    while (true) {
        const auto currentTime{static_cast<float>(glfwGetTime())};
        const auto deltaTime{currentTime - lastFrameTime};
        lastFrameTime = currentTime;

        if (window->getKeyState(GLFW_KEY_ESCAPE)) {
            window->close();
            return;
        }

        window->preUpdate();

        update();
        render();

        window->postUpdate();
    }
}
