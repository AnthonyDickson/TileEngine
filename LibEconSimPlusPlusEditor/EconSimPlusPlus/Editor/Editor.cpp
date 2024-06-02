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
#include <thread>
#include <utility>

#include "glm/ext/matrix_transform.hpp"
#include "portable-file-dialogs.h"

#include <EconSimPlusPlus/Button.hpp>
#include <EconSimPlusPlus/Editor/Editor.hpp>
#include <EconSimPlusPlus/Editor/OpenFileDialog.hpp>
#include <EconSimPlusPlus/FrameTimer.hpp>
#include <EconSimPlusPlus/GridLines.hpp>

// TODO: Open texture from disk and create an empty tile map with grid lines overlay.
namespace EconSimPlusPlus::Editor {

    Editor::Editor(std::unique_ptr<Window> window) :
        m_window(std::move(window)),
        m_camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                 {0.0f, 0.0f, 100.0f}} {
    }

    Editor Editor::create(glm::ivec2 windowSize) {
        auto window{std::make_unique<Window>(windowSize.x, windowSize.y, "EconSimPlusPlus")};

        return Editor{std::move(window)};
    }

    void Editor::addObject(GameObject* object) {
        objects.push_back(object);
    }

    void Editor::update(const float deltaTime) {
        if (m_openFileDialog.active()) {
            m_openFileDialog.update();
            return;
        }

        if (m_window->hasWindowSizeChanged()) {
            m_camera.onWindowResize({static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
        }

        const InputState input{m_window->inputState()};
        m_camera.update(deltaTime, input);

        for (const auto& object : objects) {
            object->update(deltaTime, input, m_camera);
        }

        for (const auto& object : guiObjects) {
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

        for (const auto& object : guiObjects) {
            object->render(m_camera);
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
        Button testButton{buttonText,
                          topLeft,
                          {.outlineColor = glm::vec3{0.3f}, .outlineThickness = 2.0f, .anchor = Anchor::topLeft},
                          [&] {
                              std::cout << "Button pressed.\n";
                              m_openFileDialog.open(pfd::open_file("Select a file"), [](const std::string& selection) {
                                  std::cout << "User selected file " << selection << "\n";
                              });
                              // TODO: Provide file filters for images (tile sheets) and YAML (tile map).
                              // TODO: Prevent user from selecting unsupported file types
                          }};
        testButton.setLayer(98.0f);

        guiObjects.push_back(&testButton);

        GridLines gridLines{{64, 64}, {32.0f, 32.0f}};
        objects.push_back(&gridLines);

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
} // namespace EconSimPlusPlus::Editor
