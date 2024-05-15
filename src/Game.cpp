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

// TODO: Ensure auto is only used when type is written out in same line (i.e., don't use for returned types).
namespace EconSimPlusPlus {
    namespace {
        /// Convert screen space coordinates to world space coordinates.
        /// @param camera The camera to use for the coordinate space conversion.
        /// @param screenCoordinates Coordinates in screen space starting the from the top left with +y pointing down.
        /// @return 2D world coordinates.
        /// @note The centre of the viewport is mapped to the origin for the world space coordinates and a
        /// right-handed coordinate system is used.
        glm::vec2 screenToWorldCoordinates(const Camera& camera, const glm::vec2 screenCoordinates) {
            const auto [bottomLeft, topRight]{camera.viewport()};
            return {screenCoordinates.x + bottomLeft.x, -screenCoordinates.y + topRight.y};
        }
    } // namespace

    bool Game::m_isInitialised = false;

    Game::Game(std::unique_ptr<Window> window, std::unique_ptr<TileMap> tileMap, std::unique_ptr<GridLines> gridLines) :
        m_window(std::move(window)), m_tileMap(std::move(tileMap)), m_gridLines(std::move(gridLines)),
        m_camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                 {0.0f, 0.0f, 100.0f}} {
        assert(!m_isInitialised && "Cannot have more than one instance of `Game`.");
        m_isInitialised = true;
    }

    Game Game::create(glm::ivec2 windowSize) {
        auto window{std::make_unique<Window>(windowSize.x, windowSize.y, "EconSimPlusPlus")};
        auto tileMap{TileMap::create("resource/terrain.yaml")};
        auto gridLines{std::make_unique<GridLines>(tileMap->mapSize(), tileMap->tileSize())};

        return {std::move(window), std::move(tileMap), std::move(gridLines)};
    }

    void Game::update(const float deltaTime) {
        if (m_window->hasWindowSizeChanged()) {
            m_camera.onWindowResize({static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
        }

        m_camera.update(deltaTime, m_window->inputState());

        if (const InputState input{m_window->inputState()}; input.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            const glm::vec2 cursorPos{screenToWorldCoordinates(m_camera, input.getMousePosition())};

            if (m_tileMap->contains(cursorPos)) {
                std::cout << std::format("Mouse clicked over tile map at ({:.2f}, {:.2f}).\n", cursorPos.x,
                                         cursorPos.y);
            }

            if (m_gridLines->contains(cursorPos)) {
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

        m_tileMap->render(m_camera, 0.0f);
        m_gridLines->render(m_camera, 50.0f);
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

            if (m_window->inputState().getKey(GLFW_KEY_ESCAPE) or m_window->shouldClose()) {
                return;
            }

            m_window->preUpdate();
            updateTimer.startStep();
            update(timeStep);
            updateTimer.endStep();

            renderTimer.startStep();
            render();
            renderTimer.endStep();

            const auto frameTimeSummary{std::format("Update Time: {:>5.2f} ms\nRender Time: {:>5.2f} ms",
                                                    updateTimer.average(), renderTimer.average())};
            const glm::vec3 position{-static_cast<float>(m_window->width()) / 2.0f,
                                     static_cast<float>(m_window->height()) / 2.0f, 99.0f};
            m_font->render(frameTimeSummary, position, m_camera, fontSettings);

            m_window->postUpdate();
        }
    }
} // namespace EconSimPlusPlus
