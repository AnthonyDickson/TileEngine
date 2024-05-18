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
// Created by Anthony Dickson on 13/04/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_GAME_H
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_GAME_H

#include <EconSimPlusPlus/Engine/Font.hpp>
#include <EconSimPlusPlus/Engine/GridLines.hpp>
#include <EconSimPlusPlus/Engine/TileMap.hpp>
#include <EconSimPlusPlus/Engine/Window.hpp>

namespace EconSimPlusPlus::Engine {
    /// This is the main class for the program.
    class Game {
    public:
        /// Create a new game instance.
        /// @param windowSize The width and height of the window to display the game in pixels.
        /// @return A game instance.
        static Game create(glm::ivec2 windowSize);

        Game(Game&) = delete;
        Game(Game&&) = delete;

        /// Register a game object. Registered objects will be automatically updated and rendered in the main loop.
        /// @param object A pointer to the game object.
        /// @note This list and class will not manage the lifetimes of objects pointed to, that is the responsibility of
        /// the caller.
        void addObject(GameObject* object);

        /// Update the game by one step.
        /// @param deltaTime the size of the step to take in term of time (seconds).
        void update(float deltaTime);

        /// Render the game to the screen.
        void render() const;

        /// Run the main game loop (this call blocks).
        void run();

    private:
        /// Create a new game instance.
        /// @param window The window to display the game on.
        /// @param tileMap The game map made up of square tiles.
        /// @param gridLines 2D grid lines to draw over the tile map.
        Game(std::unique_ptr<Window> window, std::unique_ptr<TileMap> tileMap, std::unique_ptr<GridLines> gridLines);

        /// We only want one instance of `Game`, we use this bool to track whether an instance was already created.
        static bool m_isInitialised;

        /// The window we use to display the game.
        std::unique_ptr<Window> m_window;
        /// The game 'map'.
        std::unique_ptr<TileMap> m_tileMap;
        /// 2D grid lines.
        std::unique_ptr<GridLines> m_gridLines;
        /// A list of all game objects.
        std::vector<GameObject*> objects{};
        /// The render camera.
        Camera m_camera;
        /// The font for rendering text on screen.
        std::unique_ptr<Font> m_font{Font::create("resource/font/Roboto-Regular.ttf", {288, 288}, {64, 64}, 32.0f)};
    };
} // namespace EconSimPlusPlus::Engine

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_GAME_H
