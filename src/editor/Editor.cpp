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
// Created by Anthony Dickson on 18/05/2024.
//

#include <thread>
#include <utility>

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Editor/Editor.hpp>
#include <EconSimPlusPlus/Engine/FrameTimer.hpp>

namespace EconSimPlusPlus::Editor {

    Editor::Editor(std::unique_ptr<Engine::Window> window, std::unique_ptr<Engine::TileMap> tileMap,
                   std::unique_ptr<Engine::GridLines> gridLines) :
        m_window(std::move(window)), m_tileMap(std::move(tileMap)), m_gridLines(std::move(gridLines)),
        m_camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                 {0.0f, 0.0f, 100.0f}} {
    }

    Editor Editor::create(glm::ivec2 windowSize) {
        auto window{std::make_unique<Engine::Window>(windowSize.x, windowSize.y, "EconSimPlusPlus")};
        std::unique_ptr tileMap{Engine::TileMap::create("resource/terrain.yaml")};
        auto gridLines{std::make_unique<Engine::GridLines>(tileMap->mapSize(), tileMap->tileSize())};

        return {std::move(window), std::move(tileMap), std::move(gridLines)};
    }

    void Editor::addObject(Engine::GameObject* object) {
        objects.push_back(object);
    }

    void Editor::update(const float deltaTime) {
        if (m_window->hasWindowSizeChanged()) {
            m_camera.onWindowResize({static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
        }

        const Engine::InputState input{m_window->inputState()};
        m_camera.update(deltaTime, input);

        for (const auto& object : objects) {
            object->update(deltaTime, input, m_camera);
        }
        // TODO: Get tile map and grid lines to react to mouse over and mouse click
        // TODO: Propogate mouse click event to objects. Only send event to upper most layer object? Send cursor
        // position in both screen and world coordinates?
    }

    void Editor::render() const {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);

        for (const auto& object : objects) {
            object->render(m_camera);
        }
    }

    void Editor::run() {
        constexpr int targetFramesPerSecond{60};
        constexpr std::chrono::milliseconds targetFrameTime{1000 / targetFramesPerSecond};
        constexpr float timeStep{1.0f / targetFramesPerSecond};

        std::chrono::time_point lastFrameTime{std::chrono::steady_clock::now()};

        Engine::FrameTimer updateTimer{};
        Engine::FrameTimer renderTimer{};
        constexpr Engine::Font::RenderSettings fontSettings{.color = {1.0f, 1.0f, 0.0f},
                                                            .size = 32.0f,
                                                            .anchor = Engine::Font::Anchor::topLeft,
                                                            .outlineSize = 0.3f,
                                                            .outlineColor = {0.0f, 0.0f, 0.0f}};

        // TODO: Move this to factory function?
        m_tileMap->setLayer(1.0f);
        m_gridLines->setLayer(2.0f);
        objects.push_back(m_tileMap.get());
        objects.push_back(m_gridLines.get());

        while (true) {
            const std::chrono::time_point currentTime{std::chrono::steady_clock::now()};
            const std::chrono::duration deltaTime{currentTime - lastFrameTime};
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

            // TODO: Convert frame time summary into game object?
            const std::string frameTimeSummary{std::format("Update Time: {:>5.2f} ms\nRender Time: {:>5.2f} ms",
                                                           updateTimer.average(), renderTimer.average())};
            const glm::vec3 position{-static_cast<float>(m_window->width()) / 2.0f,
                                     static_cast<float>(m_window->height()) / 2.0f, 99.0f};
            m_font->render(frameTimeSummary, position, m_camera, fontSettings);

            m_window->postUpdate();
        }
    }
} // namespace EconSimPlusPlus::Editor
