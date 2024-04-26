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

#include <EconSimPlusPlus/Game.hpp>
#include <EconSimPlusPlus/Size.hpp>

namespace EconSimPlusPlus {
    bool Game::isInitialised = false;

    Game::Game(std::unique_ptr<Window> window_, std::shared_ptr<TileMap> tileMap_) :
        window(std::move(window_)), tileMap(std::move(tileMap_)),
        camera{{static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight())}, {0.0f, 0.0f, 3.0f}} {
        assert(!isInitialised && "Cannot have more than one instance of `Game`.");
        isInitialised = true;
    }

    Game Game::create(Size<int> windowSize) {
        auto window{std::make_unique<Window>(windowSize.width, windowSize.height, "EconSimPlusPlus")};

        const auto tileMap{TileMap::create("resource/terrain.yaml")};

        return {std::move(window), tileMap};
    }

    void Game::update(const float deltaTime) {
        if (window->hasWindowSizeChanged()) {
            camera.onWindowResize({static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight())});
        }

        camera.update(deltaTime, window->getInputState());
    }

    void Game::render() const {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);

        tileMap->render(camera);
    }

    void Game::run() {
        constexpr auto targetFramesPerSecond{60};
        constexpr std::chrono::milliseconds targetFrameTime{1000 / targetFramesPerSecond};
        constexpr auto timeStep{1.0f / targetFramesPerSecond};

        auto lastFrameTime{std::chrono::steady_clock::now()};

        while (true) {
            const auto currentTime{std::chrono::steady_clock::now()};
            const auto deltaTime{currentTime - lastFrameTime};
            lastFrameTime = currentTime;

            if (deltaTime < targetFrameTime) {
                std::this_thread::sleep_for(targetFrameTime - deltaTime);
            }

            if (window->getInputState().getKey(GLFW_KEY_ESCAPE)) {
                window->close();
                return;
            }

            window->preUpdate();
            update(timeStep);
            render();
            window->postUpdate();
        }
    }
} // namespace EconSimPlusPlus
