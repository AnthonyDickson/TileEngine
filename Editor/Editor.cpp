#include <filesystem>
#include <fstream>
#include <numeric>
#include <thread>

#include "glm/ext/matrix_transform.hpp"
#include "portable-file-dialogs.h"
#include "yaml-cpp/yaml.h"

#include <TileEngine/Button.hpp>
#include <TileEngine/Editor/MessageDialog.hpp>
#include <TileEngine/Editor/OpenFileDialog.hpp>
#include <TileEngine/Editor/SaveFileDialog.hpp>
#include <TileEngine/Event.hpp>
#include <TileEngine/FrameTimer.hpp>
#include <TileEngine/GridLines.hpp>
#include <TileEngine/Group.hpp>
#include <TileEngine/Image.hpp>
#include <TileEngine/TextField.hpp>
#include <TileEngine/TileMap.hpp>
#include <TileEngine/TileSheet.hpp>
#include <TileEngine/TwoColumnLayout.hpp>

#include "Editor.hpp"


namespace TileEngine::Editor {

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
        auto window{std::make_unique<Window>(windowSize.x, windowSize.y, "TileEngine")};

        return Editor{std::move(window)};
    }

    void Editor::run() {
        constexpr int targetFramesPerSecond{60};
        constexpr std::chrono::milliseconds targetFrameTime{1000 / targetFramesPerSecond};
        constexpr float timeStep{1.0f / targetFramesPerSecond};

        std::chrono::time_point lastFrameTime{std::chrono::steady_clock::now()};

        FrameTimer updateTimer{};
        FrameTimer renderTimer{};
        Text frameTimeText{
            "", m_guiGraphics.font.get(),
            Font::Style{
                .color = {1.0f, 1.0f, 0.0f}, .size = 32.0f, .outlineSize = 0.3f, .outlineColor = {0.0f, 0.0f, 0.0f}}};
        frameTimeText.setAnchor(Anchor::topRight);
        frameTimeText.setLayer(99.0f);

        Button::Style buttonStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.9f}};
        Button::Style buttonActiveStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.8f}};
        Button::Style buttonDisabledStyle{.textColor = glm::vec3{0.4f}, .fillColor = glm::vec3{0.5f}};

        auto openFile = [&] {
            m_dialog = std::make_unique<OpenFileDialog>(
                pfd::open_file("Select a file", ".", {"Image Files", "*.png *.jpg *.jpeg"}),
                [this](const std::string& selection) { getTileSize(selection); });
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

            m_dialog = {std::make_unique<MessageDialog>(
                pfd::message("Unsaved Changes",
                             "Any unsaved changes will be lost if you open a new file. Do you want to continue?",
                             pfd::choice::yes_no, pfd::icon::warning),
                openFile, [&] {})};
        };

        Text buttonText{"Open...", m_guiGraphics.font.get(), {.size = 32.0f}};
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

        m_guiObjects.push_back(openFileButton);

        Text saveButtonText{"Save...", m_guiGraphics.font.get(), {.size = 32.0f}};
        auto saveFileButton{std::make_shared<Button>(
            saveButtonText,
            [&] {
                m_dialog = std::make_unique<SaveFileDialog>(
                    pfd::save_file("Select a file", "", {"YAML Files", "*.yaml"}),
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
        m_guiObjects.push_back(saveFileButton);

        // Code for testing.
        getTileSize("resource/basic_tileset_and_assets_standard/terrain_tiles_v2.png");
        // loadTileSheet(Image::create("resource/basic_tileset_and_assets_standard/terrain_tiles_v2.png"),
        // glm::vec2{32.0f, 32.0f});

        while (true) {
            const std::chrono::time_point currentTime{std::chrono::steady_clock::now()};
            const std::chrono::duration deltaTime{currentTime - lastFrameTime};
            lastFrameTime = currentTime;

            if (deltaTime < targetFrameTime) {
                std::this_thread::sleep_for(targetFrameTime - deltaTime);
            }

            if (m_window->shouldClose()) {
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
            frameTimeText.render(m_guiGraphics);

            m_window->postUpdate();
        }
    }

    Editor::Editor(std::unique_ptr<Window> window) :
        m_window(std::move(window)),
        m_graphics(Graphics{
            .camera = Camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                             {0.0f, 0.0f, 100.0f}},
        }),
        m_guiGraphics(Graphics{
            .camera = Camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                             {0.0f, 0.0f, 100.0f}},
        }) {
    }

    void Editor::getTileSize(const std::string& filepath) {
        std::erase(m_guiObjects, m_tileSizeForm);
        constexpr glm::vec2 defaultTileSize{32.0f, 32.0f};
        const Image::Image image{Image::create(filepath)};

        // TODO: Add text that describes what they need to do and perhaps also why?

        m_tileSizeForm = std::make_shared<Group>(
            Group::Layout{.direction = Group::LayoutDirection::vertical,
                          .horizontalAlignment = Group::HorizontalAlignment::center,
                          .verticalAlignment = Group::VerticalAlignment::center},
            Group::Style{.fillColor = glm::vec4{glm::vec3{0.2f}, 0.5f}});
        m_tileSizeForm->setAnchor(Anchor::center);
        m_tileSizeForm->setSize(m_window->size());
        m_tileSizeForm->setPosition(glm::vec2{0.0f});
        m_tileSizeForm->setLayer(99.0f);

        auto innerFormGroup = std::make_shared<Group>(
            Group::Layout{.direction = Group::LayoutDirection::vertical,
                          .padding = glm::vec2{8.0f},
                          .spacing = 8.0f,
                          .horizontalAlignment = Group::HorizontalAlignment::justified,
                          .verticalAlignment = Group::VerticalAlignment::justified},
            Group::Style{.outline = Outline::Style{.color = glm::vec4{0.5f, 0.5f, 0.5f, 1.0f}, .thickness = 2.0f},
                         .fillColor = glm::vec4{glm::vec3{1.0f}, 1.0f}});

        const auto tileSheetContainer{
            std::make_shared<Group>(Group::Layout{.direction = Group::LayoutDirection::horizontal,
                                                  .horizontalAlignment = Group::HorizontalAlignment::justified,
                                                  .verticalAlignment = Group::VerticalAlignment::justified})};

        const auto textFieldValidator = [&](const std::string& text) {
            auto showDialog = [&](const std::string& message) {
                m_dialog = {std::make_unique<MessageDialog>(
                    pfd::message("Invalid Input", message, pfd::choice::ok, pfd::icon::warning), [] {}, [] {})};
            };

            try {
                if (const int number{std::stoi(text)}; number < 1) {
                    showDialog(std::format("Please enter a positive number (you entered {:d}).", number));
                    return false;
                }

                return true;
            }
            catch (std::invalid_argument&) {
                showDialog(
                    std::format("Please enter a number ({:s} is not a number).", text.empty() ? "empty string" : text));
                return false;
            }
            catch (std::out_of_range&) {
                showDialog(std::format("Please enter a smaller number, the number {:s} is too big.", text));
                return false;
            }
        };

        // Tile Sheet Display
        const auto createTileSheetDisplay = [=](glm::vec2 tileSize) {
            auto tileSheet = std::make_unique<TileSheet>(Texture::create(image), tileSize);
            std::vector<int> tiles(tileSheet->tileCount());
            std::iota(tiles.begin(), tiles.end(), 1);
            const auto tileMap{std::make_shared<TileMap>(std::move(tileSheet), tileSheet->sheetSize(), tiles)};
            tileMap->enableGridLines();

            return tileMap;
        };

        m_tileSizeSheet = createTileSheetDisplay(defaultTileSize);

        const auto tileSizeGroup{std::make_shared<TwoColumnLayout>(
            TwoColumnLayout::Layout{.padding = glm::vec2{0.0f}, .spacing = glm::vec2{4.0f}})};

        tileSizeGroup->addChild(std::make_shared<Text>("Tile Size", m_graphics.font.get(), Font::Style{}));

        const auto tileWidthLabel{std::make_shared<Text>("Width: ", m_graphics.font.get(), Font::Style{})};
        auto tileWidthTextField{std::make_shared<TextField>(
            "0", m_graphics.font.get(), TextField::Config{.maxLength = 3, .mode = TextField::Mode::numeric},
            TextField::Style{})};
        tileWidthTextField->setText(std::to_string(static_cast<int>(defaultTileSize.x)));
        tileWidthTextField->setInputValidator(textFieldValidator);
        tileWidthTextField->setSubmitAction([=](const std::string& text) {
            const int tileWidth{std::stoi(text)};
            tileSheetContainer->removeChild(m_tileSizeSheet);
            m_tileSizeSheet = createTileSheetDisplay(glm::vec2{tileWidth, m_tileSizeSheet->tileSize().y});
            tileSheetContainer->addChild(m_tileSizeSheet);
            tileWidthTextField->notify(Event::defocus, EventData{*m_window});
            m_focusedObject = nullptr;
        });
        tileWidthTextField->setFocusable(true);
        tileSizeGroup->addRow(tileWidthLabel, tileWidthTextField);

        const auto tileHeightLabel{std::make_shared<Text>("Height: ", m_graphics.font.get(), Font::Style{})};
        auto tileHeightTextField = std::make_shared<TextField>(
            "0", m_graphics.font.get(), TextField::Config{.maxLength = 3, .mode = TextField::Mode::numeric},
            TextField::Style{});
        tileHeightTextField->setText(std::to_string(static_cast<int>(defaultTileSize.y)));
        tileHeightTextField->setInputValidator(textFieldValidator);
        tileHeightTextField->setSubmitAction([=](const std::string& text) {
            const int tileHeight{std::stoi(text)};
            tileSheetContainer->removeChild(m_tileSizeSheet);
            m_tileSizeSheet = createTileSheetDisplay(glm::vec2{m_tileSizeSheet->tileSize().x, tileHeight});
            tileSheetContainer->addChild(m_tileSizeSheet);
            tileHeightTextField->notify(Event::defocus, EventData{*m_window});
            m_focusedObject = nullptr;
        });
        tileHeightTextField->setFocusable(true);
        tileSizeGroup->addRow(tileHeightLabel, tileHeightTextField);
        tileSheetContainer->addChild(tileSizeGroup);

        // Tile sheet display
        tileSheetContainer->addChild(m_tileSizeSheet);

        innerFormGroup->addChild(tileSheetContainer);

        const auto buttonContainer{
            std::make_shared<Group>(Group::Layout{.direction = Group::LayoutDirection::horizontal,
                                                  .spacing = 8.0f,
                                                  .horizontalAlignment = Group::HorizontalAlignment::right,
                                                  .verticalAlignment = Group::VerticalAlignment::justified})};

        Button::Style buttonStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.9f}};
        Button::Style buttonActiveStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.8f}};
        Button::Style buttonDisabledStyle{.textColor = glm::vec3{0.4f}, .fillColor = glm::vec3{0.5f}};

        Text buttonText{"Confirm", m_graphics.font.get(), {.size = 32.0f}};
        auto confirmButton{std::make_shared<Button>(
            buttonText,
            [=] {
                std::erase(m_guiObjects, m_tileSizeForm);
                m_tileSizeForm = nullptr;
                m_tileSizeSheet = nullptr;
                loadTileSheet(image,
                              glm::vec2{std::stof(tileWidthTextField->text()), std::stof(tileHeightTextField->text())});
            },
            buttonStyle, buttonActiveStyle, buttonDisabledStyle)};
        buttonContainer->addChild(confirmButton);

        buttonText = {"Cancel", m_graphics.font.get(), {.size = 32.0f}};
        auto cancelFileButton{std::make_shared<Button>(
            buttonText,
            [=] {
                std::erase(m_guiObjects, m_tileSizeForm);
                m_tileSizeForm = nullptr;
                m_tileSizeSheet = nullptr;
            },
            buttonStyle, buttonActiveStyle, buttonDisabledStyle)};
        buttonContainer->addChild(cancelFileButton);

        buttonContainer->setSize(glm::vec2{innerFormGroup->size().x, buttonContainer->size().y});
        innerFormGroup->addChild(buttonContainer);

        m_tileSizeForm->addChild(innerFormGroup);

        m_guiObjects.push_back(m_tileSizeForm);
    }

    void Editor::loadTileSheet(const Image::Image& image, glm::vec2 tileSize) {
        // TODO: Show dialog that asks user whether save current tile map, discard any changes or cancel the open
        // operation.
        constexpr glm::ivec2 defaultMapSize{16, 16};
        const std::vector defaultTiles(defaultMapSize.x * defaultMapSize.y, 0);

        m_selectedTileID = 0;
        std::erase(m_gameObjects, m_tileMap);
        std::erase(m_guiObjects, m_tileSheetPanel);

        // Tile map display
        auto tileSheet{std::make_unique<TileSheet>(Texture::create(image), tileSize)};
        m_tileMap = std::make_shared<TileMap>(std::move(tileSheet), defaultMapSize, defaultTiles);
        m_tileMap->setAnchor(Anchor::center);
        m_tileMap->setLayer(1.0f);
        m_tileMap->enableGridLines();
        m_tileMap->addClickListener(
            [&](const glm::ivec2 gridCoordinates, int) { m_tileMap->setTileID(gridCoordinates, m_selectedTileID); });
        m_gameObjects.push_back(m_tileMap);

        // Side panel
        m_tileSheetPanel = std::make_shared<Group>(
            Group::Layout{.direction = Group::LayoutDirection::vertical, .padding = glm::vec2{8.0f}, .spacing = 4.0f},
            Group::Style{.fillColor = glm::vec4{1.0f},
                         .outline = Outline::Style{.color = glm::vec4{0.5f, 0.5f, 0.5f, 1.0f},
                                                   .thickness = 1.0f,
                                                   Outline::Placement::outset}});
        m_tileSheetPanel->setAnchor(Anchor::topRight);
        m_tileSheetPanel->setPosition(topRight(*m_window));
        m_tileSheetPanel->setLayer(10.0f);
        m_tileSheetPanel->addEventHandler([&](const Event event, const EventData& eventData) {
            if (event == Event::windowResize) {
                m_tileSheetPanel->setPosition(topRight(eventData.window));
            }
        });
        m_guiObjects.push_back(m_tileSheetPanel);

        const auto textFieldValidator = [&](const std::string& text) {
            auto showDialog = [&](const std::string& message) {
                m_dialog = {std::make_unique<MessageDialog>(
                    pfd::message("Invalid Input", message, pfd::choice::ok, pfd::icon::warning), [] {}, [] {})};
            };

            try {
                if (const int number{std::stoi(text)}; number < 1) {
                    showDialog(std::format("Please enter a positive number (you entered {:d}).", number));
                    return false;
                }

                return true;
            }
            catch (std::invalid_argument&) {
                showDialog(
                    std::format("Please enter a number ({:s} is not a number).", text.empty() ? "empty string" : text));
                return false;
            }
            catch (std::out_of_range&) {
                showDialog(std::format("Please enter a smaller number, the number {:s} is too big.", text));
                return false;
            }
        };

        const auto mapSizeGroup{std::make_shared<TwoColumnLayout>(
            TwoColumnLayout::Layout{.padding = glm::vec2{0.0f}, .spacing = glm::vec2{4.0f}})};

        mapSizeGroup->addChild(std::make_shared<Text>("Map Size", m_guiGraphics.font.get(), Font::Style{}));

        const auto mapWidthLabel{std::make_shared<Text>("Width: ", m_guiGraphics.font.get(), Font::Style{})};
        auto mapWidthTextField{std::make_shared<TextField>(
            "0", m_guiGraphics.font.get(), TextField::Config{.maxLength = 3, .mode = TextField::Mode::numeric},
            TextField::Style{})};
        mapWidthTextField->setText(std::to_string(m_tileMap->mapSize().x));
        mapWidthTextField->setInputValidator(textFieldValidator);
        mapWidthTextField->setSubmitAction([&, mapWidthTextField](const std::string& text) {
            m_tileMap->setMapSize(glm::ivec2{std::stoi(text), m_tileMap->mapSize().y});

            if (mapWidthTextField.get() == m_focusedObject) {
                mapWidthTextField->notify(Event::defocus, {*m_window});
                m_focusedObject = nullptr;
            }
        });
        mapWidthTextField->setFocusable(true);
        mapSizeGroup->addRow(mapWidthLabel, mapWidthTextField);

        const auto mapHeightLabel{std::make_shared<Text>("Height: ", m_guiGraphics.font.get(), Font::Style{})};
        auto mapHeightTextField = std::make_shared<TextField>(
            "0", m_guiGraphics.font.get(), TextField::Config{.maxLength = 3, .mode = TextField::Mode::numeric},
            TextField::Style{});
        mapHeightTextField->setText(std::to_string(m_tileMap->mapSize().y));
        mapHeightTextField->setInputValidator(textFieldValidator);
        mapHeightTextField->setSubmitAction([&, mapHeightTextField](const std::string& text) {
            m_tileMap->setMapSize(glm::ivec2{m_tileMap->mapSize().x, std::stoi(text)});

            if (mapHeightTextField.get() == m_focusedObject) {
                mapHeightTextField->notify(Event::defocus, {*m_window});
                m_focusedObject = nullptr;
            }
        });
        mapHeightTextField->setFocusable(true);
        mapSizeGroup->addRow(mapHeightLabel, mapHeightTextField);
        m_tileSheetPanel->addChild(mapSizeGroup);

        // Tile sheet display
        tileSheet = std::make_unique<TileSheet>(Texture::create(image), tileSize);
        std::vector<int> tiles(tileSheet->tileCount());
        std::iota(tiles.begin(), tiles.end(), 1);
        const auto tileMap{std::make_shared<TileMap>(std::move(tileSheet), tileSheet->sheetSize(), tiles)};
        tileMap->enableGridLines();
        tileMap->addClickListener([&](glm::ivec2, const int tileID) { m_selectedTileID = tileID; });
        m_tileSheetPanel->addChild(tileMap);
        // TODO: Add callback to tile map for when a tile is: 1) hovered over (e.g., highlight the square outline).
        // TODO: When a tile is hovered over in the tile map, that tile should be highlighted with an outline.

        // TODO: Add GUI element that shows currently selected tile.
        // TODO: Add undo/redo functionality.
        // TODO: Paint tiles by holding down mouse button and moving over grid cells in addition to single clicks.
        // TODO: 'Color picking' tool where right clicking on a tile will select that tile for painting.

        notifyAll(Event::tileMapLoaded);
    }

    void Editor::update(const float deltaTime) {
        if (m_window->hasWindowSizeChanged()) {
            m_graphics.camera.onWindowResize(
                {static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
            notifyAll(Event::windowResize);
            m_guiGraphics.camera.onWindowResize(
                {static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
            notifyAll(Event::windowResize);
        }

        if (m_dialog != nullptr and m_dialog->active()) {
            m_dialog->update();
            return;
        }

        const InputState input{m_focusedObject == nullptr ? m_window->inputState()
                                                          : m_window->inputState().withoutKeyboardInput()};

        std::vector<std::shared_ptr<Object>> gameObjects;
        std::vector<std::shared_ptr<Object>> guiObjects;

        if (m_tileSizeForm == nullptr) {
            m_graphics.camera.update(deltaTime, input);

            gameObjects = m_gameObjects;
            guiObjects = m_guiObjects;
        }
        else {
            gameObjects = {};
            guiObjects = {m_tileSizeForm};
        }

        std::ranges::sort(m_gameObjects, [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
            return a->layer() > b->layer();
        });

        for (const auto& object : gameObjects) {
            object->update(deltaTime, input, m_graphics.camera);
        }

        const Camera guiCamera{atOrigin(m_graphics.camera)};

        std::ranges::sort(guiObjects, [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
            return a->layer() > b->layer();
        });

        for (const auto& object : guiObjects) {
            if (object.get() != m_focusedObject) {
                object->update(deltaTime, input, guiCamera);
            }
        }

        const std::unordered_set handledEvents{handleEvents(guiObjects, guiCamera, m_window->inputState(), {})};
        handleEvents(gameObjects, m_graphics.camera, m_window->inputState(), handledEvents);
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void Editor::notifyAll(const Event event) {
        for (const auto& object : m_gameObjects) {
            object->notify(event, {*m_window});
        }

        for (const auto& object : m_guiObjects) {
            object->notify(event, {*m_window});
        }
    }

    std::unordered_set<Event> Editor::handleEvents(const std::vector<std::shared_ptr<Object>>& objects,
                                                   const Camera& camera, const InputState& inputState,
                                                   const std::unordered_set<Event>& triggeredEvents) {
        std::vector sortedObjects{objects};

        std::ranges::sort(sortedObjects, [&](const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {
            return a->layer() > b->layer();
        });

        const glm::vec2 cursorPos{screenToWorldCoordinates(inputState.mousePosition(), camera)};
        const glm::vec2 previousCursorPos{
            screenToWorldCoordinates(inputState.mousePosition() + inputState.mouseMovement(), camera)};

        std::unordered_set updatedTriggeredEvents{triggeredEvents};

        std::optional<int> keyDownCode{std::nullopt};

        for (int key = GLFW_KEY_SPACE; key < GLFW_KEY_LAST; ++key) {
            if (inputState.keyDown(key)) {
                keyDownCode = key;
                break;
            }
        }

        for (const auto& object : traverse(sortedObjects)) {
            if (not updatedTriggeredEvents.contains(Event::mouseEnter) and contains(*object, cursorPos) and
                not contains(*object, previousCursorPos)) {
                object->notify(Event::mouseEnter, {*m_window});
                updatedTriggeredEvents.insert(Event::mouseEnter);
            }

            if (not updatedTriggeredEvents.contains(Event::mouseLeave) and not contains(*object, cursorPos) and
                contains(*object, previousCursorPos)) {
                object->notify(Event::mouseLeave, {*m_window});
                updatedTriggeredEvents.insert(Event::mouseLeave);
            }

            if (not updatedTriggeredEvents.contains(Event::mouseHover) and
                not inputState.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and contains(*object, cursorPos)) {
                object->notify(Event::mouseHover, {*m_window, cursorPos});
                updatedTriggeredEvents.insert(Event::mouseHover);
            }

            if (not updatedTriggeredEvents.contains(Event::mouseClick) and
                inputState.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and contains(*object, cursorPos)) {
                object->notify(Event::mouseClick, {*m_window, cursorPos});
                updatedTriggeredEvents.insert(Event::mouseClick);
            }

            if (not updatedTriggeredEvents.contains(Event::focus) and
                inputState.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and object.get() != m_focusedObject and
                object->focusable() and contains(*object, cursorPos)) {
                object->notify(Event::focus, {*m_window});

                // The focused object pointer is overwritten in this event handler. So if the previously focused object
                // appears later in the objects list, the defocus event will not be triggered correctly for the
                // previously focused object. Therefore we check here before changing the pointer to ensure the defocus
                // event is triggered correctly.
                if (m_focusedObject != nullptr) {
                    m_focusedObject->notify(Event::defocus, {*m_window});
                    updatedTriggeredEvents.insert(Event::defocus);
                }

                m_focusedObject = object.get();
                updatedTriggeredEvents.insert(Event::focus);
            }

            if (not updatedTriggeredEvents.contains(Event::defocus) and m_focusedObject != nullptr and
                object.get() == m_focusedObject and
                (m_window->inputState().keyDown(GLFW_KEY_ESCAPE) or
                 inputState.mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and not contains(*m_focusedObject, cursorPos))) {
                m_focusedObject->notify(Event::defocus, {*m_window});
                m_focusedObject = nullptr;
                updatedTriggeredEvents.insert(Event::defocus);
            }

            if (not updatedTriggeredEvents.contains(Event::keyDown) and keyDownCode.has_value() and
                (m_focusedObject == nullptr or object.get() == m_focusedObject)) {
                object->notify(Event::keyDown, {*m_window, std::nullopt, *keyDownCode, inputState.keyModifiers()});
                updatedTriggeredEvents.insert(Event::keyDown);
            }
        }

        return updatedTriggeredEvents;
    }

    void Editor::render() const {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);

        for (const auto& object : m_gameObjects) {
            object->render(m_graphics);
        }

        for (const auto& object : m_guiObjects) {
            object->render(m_guiGraphics);
        }
    }
} // namespace TileEngine::Editor
