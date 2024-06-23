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

#include <filesystem>
#include <fstream>
#include <numeric>
#include <thread>

#include "glm/ext/matrix_transform.hpp"
#include "portable-file-dialogs.h"
#include "yaml-cpp/yaml.h"

#include <EconSimPlusPlus/Button.hpp>
#include <EconSimPlusPlus/Editor/Editor.hpp>
#include <EconSimPlusPlus/Editor/OpenFileDialog.hpp>
#include <EconSimPlusPlus/Event.hpp>
#include <EconSimPlusPlus/FrameTimer.hpp>
#include <EconSimPlusPlus/GridLines.hpp>
#include <EconSimPlusPlus/Panel.hpp>
#include <EconSimPlusPlus/TextField.hpp>
#include <EconSimPlusPlus/TileMap.hpp>
#include <EconSimPlusPlus/TileSheet.hpp>

namespace EconSimPlusPlus::Editor {

    namespace {
        void save(const TileMap* tileMap, const std::string& path) {
            YAML::Emitter yamlOut{};

            yamlOut << YAML::BeginDoc;
            yamlOut << YAML::BeginMap;
            yamlOut << YAML::Key << "tile-sheet";
            yamlOut << YAML::Value;
            {
                yamlOut << YAML::BeginMap;
                yamlOut << YAML::Key << "path";
                yamlOut << YAML::Value
                        << std::filesystem::relative({tileMap->texturePath()}, std::filesystem::current_path());
                yamlOut << YAML::Key << "tile-size" << YAML::Comment("in pixels");
                yamlOut << YAML::Value;
                {
                    yamlOut << YAML::BeginMap;
                    yamlOut << YAML::Key << "width" << YAML::Value << tileMap->tileSize().x;
                    yamlOut << YAML::Key << "height" << YAML::Value << tileMap->tileSize().y;
                    yamlOut << YAML::EndMap; // tile-size
                }
                yamlOut << YAML::EndMap; // tile-sheet
            }
            yamlOut << YAML::Key << "tile-map";
            yamlOut << YAML::Value;
            {
                yamlOut << YAML::BeginMap;
                yamlOut << YAML::Key << "width" << YAML::Value << tileMap->mapSize().x << YAML::Comment("in tiles");
                yamlOut << YAML::Key << "height" << YAML::Value << tileMap->mapSize().y << YAML::Comment("in tiles");
                yamlOut << YAML::Key << "tiles" << YAML::Value << YAML::Flow << tileMap->tiles();
                yamlOut << YAML::EndMap; // tile-map
            }
            yamlOut << YAML::EndMap;
            yamlOut << YAML::EndDoc;

            std::filesystem::path yamlRelativePath{std::filesystem::relative({path}, std::filesystem::current_path())};

            if (!yamlRelativePath.has_extension()) {
                yamlRelativePath.replace_extension("yaml");
            }

            std::ofstream yamlFile(yamlRelativePath.generic_string());
            yamlFile << yamlOut.c_str() << std::endl;
            yamlFile.close();
        }
    } // namespace

    Editor Editor::create(glm::ivec2 windowSize) {
        auto window{std::make_unique<Window>(windowSize.x, windowSize.y, "EconSimPlusPlus")};

        return Editor{std::move(window)};
    }

    void Editor::update(const float deltaTime) {
        if (m_openFileDialog.active()) {
            m_openFileDialog.update();
            return;
        }

        if (m_saveFileDialog.active()) {
            m_saveFileDialog.update();
            return;
        }

        if (m_messageDialog.active()) {
            m_messageDialog.update();
            return;
        }

        if (m_window->hasWindowSizeChanged()) {
            m_camera.onWindowResize({static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
            m_GuiCamera.onWindowResize({static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
            notify(Event::windowResize);
        }

        if (m_exclusiveKeyboardInputTarget != nullptr) {
            m_exclusiveKeyboardInputTarget->update(deltaTime, m_window->inputState(), m_GuiCamera);
        }

        const InputState input{m_exclusiveKeyboardInputTarget == nullptr
                                   ? m_window->inputState()
                                   : m_window->inputState().withoutKeyboardInput()};
        m_camera.update(deltaTime, input);

        if (m_tileMap != nullptr) {
            m_tileMap->update(deltaTime, input, m_camera);
        }

        const glm::vec2 cursorPos{screenToWorldCoordinates(input.mousePosition(), m_GuiCamera)};
        const glm::vec2 previousCursorPos{
            screenToWorldCoordinates(input.mousePosition() + input.mouseMovement(), m_GuiCamera)};

        for (const auto& object : m_objects) {
            if (m_exclusiveKeyboardInputTarget != nullptr and object.get() != m_exclusiveKeyboardInputTarget) {
                continue; // Avoid a double update.
            }

            object->update(deltaTime, input, m_GuiCamera);
        }

        // TODO: Make sure tile map is in objects list to ensure it receives event notifications.
        // Detect and notify objects about mouse events.
        std::ranges::sort(m_objects, [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
            return a->layer() > b->layer();
        });

        for (const auto& object : m_objects) {
            if (object->contains(cursorPos) and not object->contains(previousCursorPos)) {
                object->notify(Event::mouseEnter, *m_window);
                break;
            }
        }

        for (const auto& object : m_objects) {
            if (not object->contains(cursorPos) and object->contains(previousCursorPos)) {
                object->notify(Event::mouseLeave, *m_window);
                break;
            }
        }

        for (const auto& object : m_objects) {
            if (not input.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and object->contains(cursorPos)) {
                object->notify(Event::mouseHover, *m_window);
                break;
            }
        }

        for (const auto& object : m_objects) {
            if (input.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and object->contains(cursorPos)) {
                object->notify(Event::mouseClick, *m_window);
                break;
            }
        }
    }

    void Editor::render() const {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);

        if (m_tileMap != nullptr) {
            m_tileMap->render(m_camera);
        }

        for (const auto& object : m_objects) {
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
        frameTimeText.setLayer(99.0f);

        ButtonStyle buttonStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.9f}};
        ButtonStyle buttonActiveStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.8f}};
        ButtonStyle buttonDisabledStyle{.textColor = glm::vec3{0.4f}, .fillColor = glm::vec3{0.5f}};

        auto openFile = [&] {
            m_openFileDialog.open(pfd::open_file("Select a file", ".", {"Image Files", "*.png *.jpg *.jpeg"}),
                                  [this](const std::string& selection) { loadTileSheet(selection); });
            // This ensures the cursor is restored even if the mouseLeave event is not triggered for the button that
            // opened the file dialog.
            m_window->setCursor(GLFW_CURSOR_NORMAL);
        };

        auto openFileButtonCallback = [&] {
            if (m_tileSheetPanel == nullptr) {
                openFile();
                return;
            }

            // TODO: Check whether there are unsaved changes (dirty flag?)

            m_messageDialog.open(
                pfd::message("Unsaved Changes",
                             "Any unsaved changes will be lost if you open a new file. Do you want to continue?",
                             pfd::choice::yes_no, pfd::icon::warning),
                openFile, [&] {});
        };

        Text buttonText{"Open...", m_font.get(), {.size = 32.0f, .padding = glm::vec2{16.0f}}};
        auto openFileButton{std::make_shared<Button>(buttonText, topLeft(*m_window), Anchor::topLeft,
                                                     openFileButtonCallback, buttonStyle, buttonActiveStyle,
                                                     buttonDisabledStyle)};
        openFileButton->setLayer(98.0f);
        openFileButton->addEventHandler([&](const Event event, const Window& window) {
            if (event == Event::windowResize) {
                openFileButton->setPosition(topLeft(window));
            }
        });

        m_objects.push_back(openFileButton);

        Text saveButtonText{"Save...", m_font.get(), {.size = 32.0f, .padding = glm::vec2{16.0f}}};
        auto saveFileButton{std::make_shared<Button>(
            saveButtonText, topLeft(*m_window) + glm::vec2{openFileButton->size().x + 8.0f, 0.0f}, Anchor::topLeft,
            [&] {
                m_saveFileDialog.open(pfd::save_file("Select a file", "", {"YAML Files", "*.yaml"}),
                                      [this](const std::string& filepath) { save(m_tileMap.get(), filepath); });
            },
            buttonStyle, buttonActiveStyle, buttonDisabledStyle)};
        saveFileButton->setLayer(98.0f);
        saveFileButton->setEnabled(false);
        saveFileButton->addEventHandler([&](const Event event, const Window& window) {
            switch (event) {
            case Event::tileMapLoaded:
                saveFileButton->setEnabled(true);
                break;
            case Event::windowResize:
                saveFileButton->setPosition(topLeft(window) + glm::vec2{openFileButton->size().x + 8.0f, 0.0f});
            default:
                break;
            }
        });
        m_objects.push_back(saveFileButton);

        auto textField = std::make_shared<TextField>(m_font.get());
        textField->setTransition(TextField::State::active,
                                 [&, textField] { m_exclusiveKeyboardInputTarget = textField.get(); });
        textField->setTransition(TextField::State::inactive, [&] { m_exclusiveKeyboardInputTarget = nullptr; });
        m_objects.push_back(textField);

        while (true) {
            const std::chrono::time_point currentTime{std::chrono::steady_clock::now()};
            const std::chrono::duration deltaTime{currentTime - lastFrameTime};
            lastFrameTime = currentTime;

            if (deltaTime < targetFrameTime) {
                std::this_thread::sleep_for(targetFrameTime - deltaTime);
            }

            if (m_window->inputState().key(GLFW_KEY_ESCAPE) or m_window->shouldClose()) {
                m_openFileDialog.kill();
                m_saveFileDialog.kill();
                return;
            }

            m_window->preUpdate();
            updateTimer.startStep();
            update(timeStep);
            updateTimer.endStep();

            renderTimer.startStep();
            render();
            renderTimer.endStep();

            const std::string frameTimeSummary{std::format("Update Time: {:>5.2f} ms\nRender Time: {:>5.2f} ms",
                                                           updateTimer.average(), renderTimer.average())};
            frameTimeText.setText(frameTimeSummary);
            frameTimeText.setPosition(topRight(*m_window));
            frameTimeText.render(m_camera);

            m_window->postUpdate();
        }
    }

    void Editor::loadTileSheet(const std::string& filepath) {
        // TODO: Show dialog that asks user whether save current tile map, discard any changes or cancel the open
        // operation.
        constexpr glm::vec2 defaultTileSize{32.0f, 32.0f};
        constexpr glm::ivec2 defaultMapSize{16, 16};
        const std::vector defaultTiles(defaultMapSize.x * defaultMapSize.y, 0);

        m_selectedTileID = 0;
        std::erase(m_objects, m_tileSheetPanel);

        // Tile map display
        auto tileSheet{std::make_unique<TileSheet>(Texture::create(filepath), defaultTileSize)};
        auto tileMap{std::make_unique<TileMap>(std::move(tileSheet), defaultMapSize, defaultTiles)};
        tileMap->setAnchor(Anchor::bottomLeft);
        tileMap->setLayer(1.0f);
        tileMap->enableGridLines();

        m_tileMap = std::move(tileMap);
        m_tileMap->addClickListener(
            [&](const glm::ivec2 gridCoordinates, int) { m_tileMap->setTileID(gridCoordinates, m_selectedTileID); });

        // Side panel
        // TODO: Add padding to panel.
        m_tileSheetPanel = std::make_shared<Panel>(
            topRight(*m_window),
            PanelSettings{.fillColor = glm::vec3{0.3f},
                          .outlineColor = glm::vec3{0.6f},
                          .outlineThickness = 1.0f,
                          .anchor = Anchor::topRight});
        m_tileSheetPanel->setLayer(10.0f);
        m_tileSheetPanel->addEventHandler([&](const Event event, const Window& window) {
            if (event == Event::windowResize) {
                m_tileSheetPanel->setPosition(topRight(window));
                // TODO: Set width to widest object instead of proportion of window width.
                m_tileSheetPanel->setSize(
                    {0.2f * static_cast<float>(window.width()), static_cast<float>(window.height())});
            }
        });

        FontSettings labelSettings{.anchor = Anchor::topLeft};
        auto mapSizeLabel{std::make_unique<Text>("Map Size", m_font.get(), labelSettings)};
        // TODO: Create GUI object that contains: Text label and editable text box
        // TODO: Create editable text field. Should consist of: background Quad, Text. Clicking on text field should
        // then allow the user to enter text. Clicking on anything else, or pressing the escape button should defocus
        // the text field and not longer allow text entry.
        auto mapWidthLabel{
            std::make_unique<Text>(std::format("Width: {:d}", defaultMapSize.x), m_font.get(), labelSettings)};
        auto mapHeightLabel{
            std::make_unique<Text>(std::format("Height: {:d}", defaultMapSize.y), m_font.get(), labelSettings)};

        // Tile sheet display
        tileSheet = std::make_unique<TileSheet>(Texture::create(filepath), defaultTileSize);
        std::vector<int> tiles(tileSheet->tileCount());
        std::iota(tiles.begin(), tiles.end(), 1);
        tileMap = std::make_unique<TileMap>(std::move(tileSheet), tileSheet->sheetSize(), tiles);
        tileMap->enableGridLines();
        tileMap->addClickListener([&](glm::ivec2, const int tileID) { m_selectedTileID = tileID; });
        // TODO: Add callback to tile map for when a tile is: 1) hovered over (e.g., highlight the square outline).
        // TODO: When a tile is hovered over in the tile map, that tile should be highlighted with an outline.

        m_tileSheetPanel->addObject(std::move(mapSizeLabel));
        m_tileSheetPanel->addObject(std::move(mapWidthLabel));
        m_tileSheetPanel->addObject(std::move(mapHeightLabel));
        m_tileSheetPanel->addObject(std::move(tileMap));
        m_objects.push_back(m_tileSheetPanel);
        // TODO: Add GUI elements to adjust tile size, map size etc.
        // TODO: Add GUI element that shows currently selected tile.
        // TODO: Add undo/redo functionality.
        // TODO: Paint tiles by holding down mouse button and moving over grid cells in addition to single clicks.
        // TODO: 'Color picking' tool where right clicking on a tile will select that tile for painting.

        notify(Event::tileMapLoaded);
    }

    Editor::Editor(std::unique_ptr<Window> window) :
        m_window(std::move(window)),
        m_camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())}, {0.0f, 0.0f, 100.0f}},
        m_GuiCamera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                    {0.0f, 0.0f, 100.0f}} {
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void Editor::notify(const Event event) {
        for (const auto& object : m_objects) {
            object->notify(event, *m_window);
        }

        if (m_tileSheetPanel != nullptr) {
            m_tileSheetPanel->notify(event, *m_window);
        }
    }

} // namespace EconSimPlusPlus::Editor
