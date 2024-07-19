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
// Created by Anthony Dickson on 15/07/2024.
//

#include <EconSimPlusPlus/Button.hpp>
#include <functional>
#include <numeric>

#include <EconSimPlusPlus/Editor/MessageDialog.hpp>
#include <EconSimPlusPlus/Editor/OpenTileMapForm.hpp>
#include <EconSimPlusPlus/Text.hpp>
#include <EconSimPlusPlus/TextField.hpp>
#include <EconSimPlusPlus/TileMap.hpp>
#include <EconSimPlusPlus/TileSheet.hpp>
#include <EconSimPlusPlus/TwoColumnLayout.hpp>


namespace EconSimPlusPlus::Editor {
    std::unique_ptr<OpenTileMapForm> OpenTileMapForm::create(const Image::Image& image, glm::vec2 initialTileSize,
                                                             const Font* font,
                                                             const std::function<void(glm::vec2 tileSize)>& onConfirm,
                                                             const std::function<void()>& onCancel) {
        auto form{std::make_unique<OpenTileMapForm>(Texture::create(image), initialTileSize, onConfirm, onCancel)};

        const auto formContainer{std::make_shared<Group>(
            Group::Layout{.direction = Group::LayoutDirection::vertical,
                          .padding = glm::vec2{8.0f},
                          .spacing = 8.0f,
                          .horizontalAlignment = Group::HorizontalAlignment::justified,
                          .verticalAlignment = Group::VerticalAlignment::justified},
            Group::Style{.outline = Outline::Style{.color = glm::vec4{0.5f, 0.5f, 0.5f, 1.0f}, .thickness = 2.0f},
                         .fillColor = glm::vec4{glm::vec3{1.0f}, 1.0f}})};

        const auto tileSheetContainer{
            std::make_shared<Group>(Group::Layout{.direction = Group::LayoutDirection::horizontal,
                                                  .horizontalAlignment = Group::HorizontalAlignment::justified,
                                                  .verticalAlignment = Group::VerticalAlignment::justified})};

        const auto textFieldValidator = [&](const std::string& text) {
            auto showDialog = [&](const std::string& message) {
                // TODO: Show dialog on invalid input.
                // m_dialog = {std::make_unique<MessageDialog>(
                // pfd::message("Invalid Input", message, pfd::choice::ok, pfd::icon::warning), [] {}, [] {})};
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

        const auto tileSizeGroup{std::make_shared<TwoColumnLayout>(
            TwoColumnLayout::Layout{.padding = glm::vec2{0.0f}, .spacing = glm::vec2{4.0f}})};

        tileSizeGroup->addChild(std::make_shared<Text>("Tile Size", font, Font::Style{}));

        const auto tileWidthLabel{std::make_shared<Text>("Width: ", font, Font::Style{})};
        auto tileWidthTextField{std::make_shared<TextField>(
            "0", font, TextField::Config{.maxLength = 3, .mode = TextField::Mode::numeric}, TextField::Style{})};
        tileWidthTextField->setText(std::to_string(static_cast<int>(initialTileSize.x)));
        tileWidthTextField->setInputValidator(textFieldValidator);
        tileWidthTextField->setSubmitAction([&, tileWidthTextField](const std::string& text) {
            // TODO: Create new tile sheet display with user defined tile size.
            // TODO: Defocus this text field.
        });
        tileWidthTextField->setFocusable(true);
        tileSizeGroup->addRow(tileWidthLabel, tileWidthTextField);

        const auto tileHeightLabel{std::make_shared<Text>("Height: ", font, Font::Style{})};
        auto tileHeightTextField = std::make_shared<TextField>(
            "0", font, TextField::Config{.maxLength = 3, .mode = TextField::Mode::numeric}, TextField::Style{});
        tileHeightTextField->setText(std::to_string(static_cast<int>(initialTileSize.y)));
        tileHeightTextField->setInputValidator(textFieldValidator);
        tileHeightTextField->setSubmitAction([&, tileHeightTextField](const std::string& text) {
            // TODO: Create new tile sheet display with user defined tile size.
            // TODO: Defocus this text field.
        });
        tileHeightTextField->setFocusable(true);
        tileSizeGroup->addRow(tileHeightLabel, tileHeightTextField);
        tileSheetContainer->addChild(tileSizeGroup);

        // Tile sheet display
        auto tileSheet = std::make_unique<TileSheet>(Texture::create(image), initialTileSize);
        std::vector<int> tiles(tileSheet->tileCount());
        std::iota(tiles.begin(), tiles.end(), 1);
        const auto tileMap{std::make_shared<TileMap>(std::move(tileSheet), tileSheet->sheetSize(), tiles)};
        tileMap->enableGridLines();
        tileSheetContainer->addChild(tileMap);

        formContainer->addChild(tileSheetContainer);

        const auto buttonContainer{
            std::make_shared<Group>(Group::Layout{.direction = Group::LayoutDirection::horizontal,
                                                  .spacing = 8.0f,
                                                  .horizontalAlignment = Group::HorizontalAlignment::right,
                                                  .verticalAlignment = Group::VerticalAlignment::justified})};

        // TODO: Add confirm button.
        Button::Style buttonStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.9f}};
        Button::Style buttonActiveStyle{.textColor = glm::vec3{0.0f}, .fillColor = glm::vec3{0.8f}};
        Button::Style buttonDisabledStyle{.textColor = glm::vec3{0.4f}, .fillColor = glm::vec3{0.5f}};

        Text buttonText{"Confirm", font, {.size = 32.0f}};
        auto confirmButton{
            std::make_shared<Button>(buttonText, [&] {}, buttonStyle, buttonActiveStyle, buttonDisabledStyle)};
        buttonContainer->addChild(confirmButton);
        // TODO: Add cancel button.
        buttonText = {"Cancel", font, {.size = 32.0f}};
        auto cancelFileButton{
            std::make_shared<Button>(buttonText, [&] {}, buttonStyle, buttonActiveStyle, buttonDisabledStyle)};
        buttonContainer->addChild(cancelFileButton);

        buttonContainer->setSize(glm::vec2{formContainer->size().x, buttonContainer->size().y});
        formContainer->addChild(buttonContainer);

        formContainer->setPosition(formContainer->position() +
                                   0.5f * glm::vec2{-formContainer->size().x, formContainer->size().y});

        form->addChild(formContainer);
        form->setSize(formContainer->size());
        form->setAnchor(Anchor::center);
        form->setPosition(glm::vec2{0.0f});

        return std::move(form);
    }

    OpenTileMapForm::OpenTileMapForm(std::unique_ptr<Texture> texture, const glm::vec2 initialTileSize,
                                     const std::function<void(glm::vec2 tileSize)>& onConfirm,
                                     const std::function<void()>& onCancel) :
        m_texture(std::move(texture)), m_tileSize(initialTileSize), m_onConfirm(onConfirm), m_onCancel(onCancel) {
    }

    void OpenTileMapForm::update(const float deltaTime, const InputState& inputState, const Camera& camera) {
        for (const std::shared_ptr<Object>& object : children()) {
            object->update(deltaTime, inputState, camera);
        }
    }

    void OpenTileMapForm::render(const Graphics& graphics) const {
        for (const std::shared_ptr<Object>& object : children()) {
            object->render(graphics);
        }
    }
} // namespace EconSimPlusPlus::Editor
