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

#ifndef EDITOR_ECONSIMPLUSPLUS_EDITOR_EDITOR_HPP
#define EDITOR_ECONSIMPLUSPLUS_EDITOR_EDITOR_HPP

#include <unordered_set>

#include <EconSimPlusPlus/Editor/Dialog.hpp>
#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/Group.hpp>
#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/TileMap.hpp>
#include <EconSimPlusPlus/Window.hpp>

namespace EconSimPlusPlus::Editor {
    /// This is the main class for the program.
    class Editor {
    public:
        /// Create a new editor instance.
        /// @param windowSize The width and height of the window to display the editor in pixels.
        /// @return An editor instance.
        static Editor create(glm::ivec2 windowSize);

        Editor(Editor&) = delete;
        Editor(Editor&&) = delete;

        /// Run the main application loop (this call blocks).
        void run();

    private:
        /// Create a new editor instance.
        /// @param window The window to display the editor.
        explicit Editor(std::unique_ptr<Window> window);

        /// Load a tile sheet image from disk and create an empty tilemap.
        /// @param filepath The path to the tile sheet image.
        void loadTileSheet(const std::string& filepath);

        /// Update the editor by one step.
        /// @param deltaTime the size of the step to take in term of time (seconds).
        void update(float deltaTime);

        /// Notify all objects of an event.
        /// @param event What happened.
        void notifyAll(Event event);

        /// Check for events and notify the relevant objects.
        std::unordered_set<Event> handleEvents(const std::vector<std::shared_ptr<Object>>& objects,
                                               const Camera& camera, const InputState& inputState,
                                               const std::unordered_set<Event>& triggeredEvents);

        /// Render the editor to the screen.
        void render() const;

        /// The window we use to display the editor.
        std::unique_ptr<Window> m_window;
        /// The render camera.
        Camera m_camera;
        /// The font for rendering text on screen.
        std::unique_ptr<Font> m_font{Font::create("resource/font/Roboto-Regular.ttf", {288, 288}, {64, 64}, 32.0f)};

        /// The exclusive receiver of keyboard input, if any.
        Object* m_focusedObject{nullptr};
        /// The tile to paint onto the tile map.
        int m_selectedTileID{0};
        /// The tile map that is currently being edited.
        std::shared_ptr<TileMap> m_tileMap{};
        /// The GUI panel that will hold the tile sheet and related GUI.
        std::shared_ptr<Group> m_tileSheetPanel{};
        /// A list of objects that 'move' with the camera.
        std::vector<std::shared_ptr<Object>> m_gameObjects{};
        /// A list of objects that do not 'move' with the camera.
        std::vector<std::shared_ptr<Object>> m_guiObjects{};
        /// Handles asynchronous dialogs.
        /// @note Possibly nullptr.
        std::unique_ptr<Dialog> m_dialog{};
    };
} // namespace EconSimPlusPlus::Editor

#endif // EDITOR_ECONSIMPLUSPLUS_EDITOR_EDITOR_HPP
