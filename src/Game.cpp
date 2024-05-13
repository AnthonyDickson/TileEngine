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
#include <thread>
#include <utility>

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/FrameTimer.hpp>
#include <EconSimPlusPlus/Game.hpp>

// TODO: Change classes to use m_ prefix for data members.
// TODO: Change classes use data member names minus the m_ prefix for getter functions.
// TODO: Ensure classes use "set" plus data member names minus the m_ prefix for setter functions.
// TODO: Ensure auto is only used when type is written out in same line (i.e., don't use for returned types).
namespace EconSimPlusPlus {
    bool Game::isInitialised = false;

    Game::Game(std::unique_ptr<Window> window_, std::unique_ptr<TileMap> tileMap_,
               std::unique_ptr<GridLines> gridLines_) :
        window(std::move(window_)), tileMap(std::move(tileMap_)), gridLines(std::move(gridLines_)),
        camera{{static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight())},
               {0.0f, 0.0f, 100.0f}} {
        assert(!isInitialised && "Cannot have more than one instance of `Game`.");
        isInitialised = true;
    }

    Game Game::create(glm::ivec2 windowSize) {
        auto window{std::make_unique<Window>(windowSize.x, windowSize.y, "EconSimPlusPlus")};
        auto tileMap{TileMap::create("resource/terrain.yaml")};
        auto gridLines{std::make_unique<GridLines>(tileMap->getMapSize(), tileMap->getTileSize())};

        return {std::move(window), std::move(tileMap), std::move(gridLines)};
    }

    void Game::update(const float deltaTime) {
        if (window->hasWindowSizeChanged()) {
            camera.onWindowResize({static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight())});
        }

        camera.update(deltaTime, window->getInputState());

        if (const InputState input{window->getInputState()}; input.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            const glm::vec2 cursorPos{camera.toWorld(input.getMousePosition())};

            if (tileMap->contains(cursorPos)) {
                std::cout << std::format("Mouse clicked over tile map at ({:.2f}, {:.2f}).\n", cursorPos.x,
                                         cursorPos.y);
            }

            if (gridLines->contains(cursorPos)) {
                std::cout << std::format("Mouse clicked over grid lines at ({:.2f}, {:.2f}).\n", cursorPos.x,
                                         cursorPos.y);
            }
        }
        // TODO: Get tile map and grid lines to react to mouse over and mouse click
        // TODO: Set up observer/observable pattern to propogate input events to game objects.
        // TODO: Propogate mouse click event to objects. Only send event to upper most layer object? Send cursor position in both screen and world coordinates?
    }

    void Game::render() const {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);

        tileMap->render(camera, 0.0f);
        gridLines->render(camera, 50.0f);
    }

    void Game::run() {
        constexpr auto targetFramesPerSecond{60};
        constexpr std::chrono::milliseconds targetFrameTime{1000 / targetFramesPerSecond};
        constexpr auto timeStep{1.0f / targetFramesPerSecond};

        auto lastFrameTime{std::chrono::steady_clock::now()};

        FrameTimer updateTimer{};
        FrameTimer renderTimer{};
        constexpr Font::RenderSettings fontSettings{.color = {1.0f, 1.0f, 0.0f},
                                                    .size = 32.0f,
                                                    .anchor = Font::Anchor::topLeft,
                                                    .outlineSize = 0.3f,
                                                    .outlineColor = {0.0f, 0.0f, 0.0f}};

        while (true) {
            const auto currentTime{std::chrono::steady_clock::now()};
            const auto deltaTime{currentTime - lastFrameTime};
            lastFrameTime = currentTime;

            if (deltaTime < targetFrameTime) {
                std::this_thread::sleep_for(targetFrameTime - deltaTime);
            }

            if (window->getInputState().getKey(GLFW_KEY_ESCAPE) or window->shouldClose()) {
                return;
            }

            window->preUpdate();
            updateTimer.startStep();
            update(timeStep);
            updateTimer.endStep();

            renderTimer.startStep();
            render();
            renderTimer.endStep();

            const auto frameTimeSummary{std::format("Update Time: {:>5.2f} ms\nRender Time: {:>5.2f} ms",
                                                    updateTimer.average(), renderTimer.average())};
            const glm::vec3 position{-static_cast<float>(window->getWidth()) / 2.0f,
                                     static_cast<float>(window->getHeight()) / 2.0f, 99.0f};
            font->render(frameTimeSummary, position, camera, fontSettings);

            window->postUpdate();
        }
    }
} // namespace EconSimPlusPlus
