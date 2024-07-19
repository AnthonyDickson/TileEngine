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
#include <EconSimPlusPlus/Graphics.hpp>
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

        /// Get the tile size from the user for a tile sheet.
        /// @param filepath The path to the tile sheet image.
        void getTileSize(const std::string& filepath);

        /// Create an empty tilemap from a tile sheet and create the editor GUI.
        /// @param image The tile sheet image to use.
        /// @param tileSize The size (width and height) of the tiles in the tile sheet in pixels.
        void loadTileSheet(const Image::Image& image, glm::vec2 tileSize);

        /// Update the editor by one step.
        /// @param deltaTime the size of the step to take in term of time (seconds).
        void update(float deltaTime);

        /// Notify all objects of an event.
        /// @param event What happened.
        void notifyAll(Event event);

        /// Check for events and notify the relevant objects.
        /// @param objects The objects to process events for.
        /// @param camera The camera to use for calculating mouse posititions relative to the objects.
        /// @param inputState The keyboard and mouse input state.
        /// @param triggeredEvents The events that have already been processed during this update step.
        /// @return The events that were processed inside the function plus those listed in the function arguments.
        std::unordered_set<Event> handleEvents(const std::vector<std::shared_ptr<Object>>& objects,
                                               const Camera& camera, const InputState& inputState,
                                               const std::unordered_set<Event>& triggeredEvents);

        /// Render the editor to the screen.
        void render() const;

        /// The window we use to display the editor.
        std::unique_ptr<Window> m_window;
        Graphics m_graphics;
        Graphics m_guiGraphics;

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
