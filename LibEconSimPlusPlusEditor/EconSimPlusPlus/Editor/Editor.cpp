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

#include <iostream>
#include <numeric>
#include <thread>
#include <utility>

#include "glm/ext/matrix_transform.hpp"
#include "portable-file-dialogs.h"

#include <EconSimPlusPlus/Button.hpp>
#include <EconSimPlusPlus/Editor/Editor.hpp>
#include <EconSimPlusPlus/Editor/OpenFileDialog.hpp>
#include <EconSimPlusPlus/FrameTimer.hpp>
#include <EconSimPlusPlus/GridLines.hpp>
#include <EconSimPlusPlus/Panel.hpp>
#include <EconSimPlusPlus/TileMap.hpp>
#include <EconSimPlusPlus/TileSheet.hpp>

namespace EconSimPlusPlus::Editor {

    Editor::Editor(std::unique_ptr<Window> window) :
        m_window(std::move(window)),
        m_camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())}, {0.0f, 0.0f, 100.0f}},
        m_GuiCamera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                    {0.0f, 0.0f, 100.0f}} {
    }

    Editor Editor::create(glm::ivec2 windowSize) {
        auto window{std::make_unique<Window>(windowSize.x, windowSize.y, "EconSimPlusPlus")};

        return Editor{std::move(window)};
    }

    void Editor::addObject(GameObject* object) {
        m_objects.push_back(object);
    }

    void Editor::update(const float deltaTime) {
        if (m_openFileDialog.active()) {
            m_openFileDialog.update();
            return;
        }

        if (m_window->hasWindowSizeChanged()) {
            m_camera.onWindowResize({static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
            m_GuiCamera.onWindowResize({static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
        }

        const InputState input{m_window->inputState()};
        m_camera.update(deltaTime, input);

        for (const auto& object : m_objects) {
            object->update(deltaTime, input, m_camera);
        }

        if (m_tileMap != nullptr) {
            m_tileMap->update(deltaTime, input, m_camera);
        }

        for (const auto& object : m_guiObjects) {
            object->update(deltaTime, input, m_GuiCamera);
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

        for (const auto& object : m_objects) {
            object->render(m_camera);
        }

        if (m_tileMap != nullptr) {
            m_tileMap->render(m_camera);
        }

        for (const auto& object : m_guiObjects) {
            object->render(m_GuiCamera);
        }
    }

    void Editor::run() {
        constexpr int targetFramesPerSecond{60};
        constexpr std::chrono::milliseconds targetFrameTime{1000 / targetFramesPerSecond};
        constexpr float timeStep{1.0f / targetFramesPerSecond};

        std::chrono::time_point lastFrameTime{std::chrono::steady_clock::now()};

        FrameTimer updateTimer{};
        FrameTimer renderTimer{};
        Text frameTimeText{"", m_font.get(),
                           FontSettings{.color = {1.0f, 1.0f, 0.0f},
                                        .size = 32.0f,
                                        .anchor = Anchor::topRight,
                                        .outlineSize = 0.3f,
                                        .outlineColor = {0.0f, 0.0f, 0.0f}}};

        glm::vec2 topLeft{-0.5f * static_cast<float>(m_window->width()), 0.5f * static_cast<float>(m_window->height())};

        Text buttonText{
            "Open...", m_font.get(), {.color = glm::vec3{0.0f}, .size = 32.0f, .padding = glm::vec2{16.0f}}};
        auto testButton{std::make_unique<Button>(
            buttonText, topLeft,
            ButtonSettings{.outlineColor = glm::vec3{0.3f}, .outlineThickness = 2.0f, .anchor = Anchor::topLeft}, [&] {
                m_openFileDialog.open(pfd::open_file("Select a file", ".", {"Image Files", "*.png *.jpg *.jpeg"}),
                                      [this](const std::string& selection) { loadTileSheet(selection); });
            })};
        testButton->setLayer(98.0f);

        m_guiObjects.push_back(std::move(testButton));

        while (true) {
            const std::chrono::time_point currentTime{std::chrono::steady_clock::now()};
            const std::chrono::duration deltaTime{currentTime - lastFrameTime};
            lastFrameTime = currentTime;

            if (deltaTime < targetFrameTime) {
                std::this_thread::sleep_for(targetFrameTime - deltaTime);
            }

            if (m_window->inputState().getKey(GLFW_KEY_ESCAPE) or m_window->shouldClose()) {
                m_openFileDialog.kill();
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
            frameTimeText.setText(frameTimeSummary);
            const glm::vec2 position{static_cast<float>(m_window->width()) / 2.0f,
                                     static_cast<float>(m_window->height()) / 2.0f};
            frameTimeText.setPosition(position);
            frameTimeText.render(m_camera);

            m_window->postUpdate();
        }
    }

    void Editor::loadTileSheet(const std::string& filepath) {
        constexpr glm::vec2 defaultTileSize{32.0f, 32.0f};
        constexpr glm::ivec2 defaultMapSize{16, 16};
        const std::vector defaultTiles(defaultMapSize.x * defaultMapSize.y, 0);

        // Tile map display
        auto tileSheet{std::make_unique<TileSheet>(Texture::create(filepath), defaultTileSize)};
        auto tileMap{std::make_unique<TileMap>(std::move(tileSheet), defaultMapSize, defaultTiles)};
        tileMap->enableGridLines();

        m_tileMap = std::move(tileMap);

        // Side panel
        auto panel{std::make_unique<Panel>(
            glm::vec2{0.5f * static_cast<float>(m_window->width()), 0.5f * static_cast<float>(m_window->height())},
            glm::vec2{0.2f * static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
            PanelSettings{.anchor = Anchor::topRight,
                          .fillColor = glm::vec3{0.3f},
                          .outlineThickness = 1.0f,
                          .outlineColor = glm::vec3{0.6f}})};

        // Tile sheet display
        tileSheet = std::make_unique<TileSheet>(Texture::create(filepath), defaultTileSize);
        std::vector<int> tiles(tileSheet->tileCount());
        std::iota(tiles.begin(), tiles.end(), 0);
        const int tilesPerRow{std::min(static_cast<int>(panel->size().x / defaultTileSize.x),
                                       static_cast<int>(tileSheet->sheetSize().x))};
        const int rows{static_cast<int>(ceil(tileSheet->tileCount() / tilesPerRow))};
        tileMap = std::make_unique<TileMap>(std::move(tileSheet), glm::vec2{tilesPerRow, rows}, tiles);
        tileMap->enableGridLines();
        tileMap->addClickListener([&](glm::ivec2 gridCoordinates, int tileID) {
            std::cout << std::format("Tile #{:d} clicked at ({:d}, {:d}).\n", tileID, gridCoordinates.x,
                                     gridCoordinates.y);
            m_selectedTileID = tileID;
        });
        // TODO: Add callback to tile map for when a tile is: 1) hovered over (e.g., highlight the square outline).
        // TODO: When a tile is clicked in the tile map, that tile should be assigned the tileID stored in the editor.

        panel->addObject(std::move(tileMap));

        m_guiObjects.push_back(std::move(panel));
        // TODO: Add GUI elements to adjust tile size, map size etc.
    }
} // namespace EconSimPlusPlus::Editor
