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

#ifndef EDITOR_H
#define EDITOR_H

#include <EconSimPlusPlus/Engine/Font.hpp>
#include <EconSimPlusPlus/Engine/GridLines.hpp>
#include <EconSimPlusPlus/Engine/Window.hpp>

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

        /// Register a game object. Registered objects will be automatically updated and rendered in the main loop.
        /// @param object A pointer to the game object.
        /// @note This list and class will not manage the lifetimes of objects pointed to, that is the responsibility of
        /// the caller.
        void addObject(Engine::GameObject* object);

        /// Update the editor by one step.
        /// @param deltaTime the size of the step to take in term of time (seconds).
        void update(float deltaTime);

        /// Render the editor to the screen.
        void render() const;

        /// Run the main application loop (this call blocks).
        void run();

    private:
        /// Create a new editor instance.
        /// @param window The window to display the editor.
        explicit Editor(std::unique_ptr<Engine::Window> window);

        /// The window we use to display the editor.
        std::unique_ptr<Engine::Window> m_window;
        /// A list of all game objects.
        std::vector<Engine::GameObject*> objects{};
        /// The render camera.
        Engine::Camera m_camera;
        /// The font for rendering text on screen.
        std::unique_ptr<Engine::Font> m_font{
            Engine::Font::create("resource/font/Roboto-Regular.ttf", {288, 288}, {64, 64}, 32.0f)};
    };
} // namespace EconSimPlusPlus::Editor

#endif // EDITOR_H
