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
            notify(Event::windowResize);
        }

        const Camera guiCamera{atOrigin(m_camera)};

        if (m_exclusiveKeyboardInputTarget != nullptr) {
            m_exclusiveKeyboardInputTarget->update(deltaTime, m_window->inputState(), guiCamera);
        }

        const InputState input{m_exclusiveKeyboardInputTarget == nullptr
                                   ? m_window->inputState()
                                   : m_window->inputState().withoutKeyboardInput()};
        m_camera.update(deltaTime, input);

        for (const auto& object : m_objects) {
            if (m_exclusiveKeyboardInputTarget != nullptr and object.get() != m_exclusiveKeyboardInputTarget) {
                continue; // Avoid a double update.
            }

            // TODO: Create a more general way to determine whether an object should be sent regular or GUI camera.
            if (object != m_tileMap) {
                object->update(deltaTime, input, guiCamera);
            }
            else {
                object->update(deltaTime, input, m_camera);
            }
        }

        std::ranges::sort(m_objects, [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
            return a->layer() > b->layer();
        });

        // TODO: Find more elegant way to determine which set of cursor positions to send to objects.
        const glm::vec2 cursorPos{screenToWorldCoordinates(input.mousePosition(), guiCamera)};
        const glm::vec2 previousCursorPos{
            screenToWorldCoordinates(input.mousePosition() + input.mouseMovement(), guiCamera)};

        const glm::vec2 cursorPosTileMap{screenToWorldCoordinates(input.mousePosition(), m_camera)};
        const glm::vec2 previousCursorPosTileMap{
            screenToWorldCoordinates(input.mousePosition() + input.mouseMovement(), m_camera)};

        for (const auto& object : m_objects) {
            if (object == m_tileMap and object->contains(cursorPosTileMap) and
                not object->contains(previousCursorPosTileMap)) {
                object->notify(Event::mouseEnter, {*m_window, cursorPosTileMap});
                break;
            }

            if (object->contains(cursorPos) and not object->contains(previousCursorPos)) {
                object->notify(Event::mouseEnter, {*m_window, cursorPos});
                break;
            }
        }

        for (const auto& object : m_objects) {
            if (object == m_tileMap and not object->contains(cursorPosTileMap) and
                object->contains(previousCursorPosTileMap)) {
                object->notify(Event::mouseLeave, {*m_window, cursorPosTileMap});
                break;
            }

            if (not object->contains(cursorPos) and object->contains(previousCursorPos)) {
                object->notify(Event::mouseLeave, {*m_window, cursorPos});
                break;
            }
        }

        for (const auto& object : m_objects) {
            if (object == m_tileMap and not input.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and
                object->contains(cursorPosTileMap)) {
                object->notify(Event::mouseHover, {*m_window, cursorPosTileMap});
                break;
            }

            if (not input.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and object->contains(cursorPos)) {
                object->notify(Event::mouseHover, {*m_window, cursorPos});
                break;
            }
        }

        for (const auto& object : m_objects) {
            if (object == m_tileMap and input.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and
                object->contains(cursorPosTileMap)) {
                object->notify(Event::mouseClick, {*m_window, cursorPosTileMap});
                break;
            }

            if (input.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and object->contains(cursorPos)) {
                object->notify(Event::mouseClick, {*m_window, cursorPos});
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

        const Camera guiCamera{atOrigin(m_camera)};

        for (const auto& object : m_objects) {
            if (object != m_tileMap) {
                object->render(guiCamera);
            }
            else {
                object->render(m_camera);
            }
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
                           Font::Style{.color = {1.0f, 1.0f, 0.0f},
                                       .size = 32.0f,
                                       .anchor = Anchor::topRight,
                                       .outlineSize = 0.3f,
                                       .outlineColor = {0.0f, 0.0f, 0.0f}}};
        frameTimeText.setLayer(99.0f);

        Button::Style buttonStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.9f}};
        Button::Style buttonActiveStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.8f}};
        Button::Style buttonDisabledStyle{.textColor = glm::vec3{0.4f}, .fillColor = glm::vec3{0.5f}};

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
        auto openFileButton{std::make_shared<Button>(buttonText, openFileButtonCallback, buttonStyle, buttonActiveStyle,
                                                     buttonDisabledStyle)};
        openFileButton->setAnchor(Anchor::topLeft);
        openFileButton->setPosition(topLeft(*m_window));
        openFileButton->setLayer(98.0f);
        openFileButton->addEventHandler([&](const Event event, const EventData& eventData) {
            if (event == Event::windowResize) {
                openFileButton->setPosition(topLeft(eventData.window));
            }
        });

        m_objects.push_back(openFileButton);

        Text saveButtonText{"Save...", m_font.get(), {.size = 32.0f, .padding = glm::vec2{16.0f}}};
        auto saveFileButton{std::make_shared<Button>(
            saveButtonText,
            [&] {
                m_saveFileDialog.open(pfd::save_file("Select a file", "", {"YAML Files", "*.yaml"}),
                                      [this](const std::string& filepath) { save(m_tileMap.get(), filepath); });
            },
            buttonStyle, buttonActiveStyle, buttonDisabledStyle)};
        saveFileButton->setAnchor(Anchor::topLeft);
        saveFileButton->setPosition(topLeft(*m_window) + glm::vec2{openFileButton->size().x + 8.0f, 0.0f});
        saveFileButton->setLayer(98.0f);
        saveFileButton->disable();
        saveFileButton->addEventHandler([&](const Event event, const EventData& eventData) {
            switch (event) {
            case Event::tileMapLoaded:
                saveFileButton->enable();
                break;
            case Event::windowResize:
                saveFileButton->setPosition(topLeft(eventData.window) +
                                            glm::vec2{openFileButton->size().x + 8.0f, 0.0f});
            default:
                break;
            }
        });
        m_objects.push_back(saveFileButton);

        auto textField = std::make_shared<TextField>(m_font.get(), TextField::Style{});
        textField->setTransition(TextField::State::active,
                                 [&, textField] { m_exclusiveKeyboardInputTarget = textField.get(); });
        textField->setTransition(TextField::State::inactive, [&] { m_exclusiveKeyboardInputTarget = nullptr; });
        textField->setLayer(50.0f);
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
            frameTimeText.render(atOrigin(m_camera));

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
        std::erase(m_objects, m_tileMap);

        // Tile map display
        auto tileSheet{std::make_unique<TileSheet>(Texture::create(filepath), defaultTileSize)};
        m_tileMap = std::make_shared<TileMap>(std::move(tileSheet), defaultMapSize, defaultTiles);
        m_tileMap->setAnchor(Anchor::bottomLeft);
        m_tileMap->setLayer(1.0f);
        m_tileMap->enableGridLines();
        m_tileMap->addClickListener(
            [&](const glm::ivec2 gridCoordinates, int) { m_tileMap->setTileID(gridCoordinates, m_selectedTileID); });
        m_objects.push_back(m_tileMap);

        // Side panel
        // TODO: Add padding to panel.
        m_tileSheetPanel = std::make_shared<Panel>(
            topRight(*m_window),
            Panel::Style{.fillColor = glm::vec3{0.3f},
                         .outline = Outline::Style{
                             .color = glm::vec3{0.6f}, .thickness = 1.0f, .placement = Outline::Placement::inset}});
        m_tileSheetPanel->setAnchor(Anchor::topRight);
        m_tileSheetPanel->setLayer(10.0f);
        m_tileSheetPanel->addEventHandler([&](const Event event, const EventData& eventData) {
            if (event == Event::windowResize) {
                m_tileSheetPanel->setPosition(topRight(eventData.window));
                // TODO: Set width to widest object instead of proportion of window width.
                m_tileSheetPanel->setSize({0.2f * static_cast<float>(eventData.window.width()),
                                           static_cast<float>(eventData.window.height())});
            }
        });

        Font::Style labelStyle{.anchor = Anchor::topLeft};
        auto mapSizeLabel{std::make_unique<Text>("Map Size", m_font.get(), labelStyle)};
        // TODO: Create GUI object that contains: Text label and editable text box
        // TODO: Create editable text field. Should consist of: background Quad, Text. Clicking on text field should
        // then allow the user to enter text. Clicking on anything else, or pressing the escape button should defocus
        // the text field and not longer allow text entry.
        auto mapWidthLabel{
            std::make_unique<Text>(std::format("Width: {:d}", defaultMapSize.x), m_font.get(), labelStyle)};
        auto mapHeightLabel{
            std::make_unique<Text>(std::format("Height: {:d}", defaultMapSize.y), m_font.get(), labelStyle)};

        // Tile sheet display
        tileSheet = std::make_unique<TileSheet>(Texture::create(filepath), defaultTileSize);
        std::vector<int> tiles(tileSheet->tileCount());
        std::iota(tiles.begin(), tiles.end(), 1);
        auto tileMap{std::make_unique<TileMap>(std::move(tileSheet), tileSheet->sheetSize(), tiles)};
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
        m_camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                 {0.0f, 0.0f, 100.0f}} {
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void Editor::notify(const Event event) {
        for (const auto& object : m_objects) {
            object->notify(event, {*m_window, std::nullopt});
        }

        if (m_tileSheetPanel != nullptr) {
            m_tileSheetPanel->notify(event, {*m_window, std::nullopt});
        }
    }

} // namespace EconSimPlusPlus::Editor
