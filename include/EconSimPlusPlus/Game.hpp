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

#ifndef GAME_H
#define GAME_H

#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/TileMap.hpp>
#include <EconSimPlusPlus/Window.hpp>

namespace EconSimPlusPlus {
    class Game {
        /// This is the main class for the program. Wraps up and coordinates everything.
    private:
        /// The window we use to display the game.
        std::unique_ptr<Window> window;

        /// The game 'map'.
        std::unique_ptr<TileMap> tileMap;
        /// The render camera.
        Camera camera;
        /// The font for rendering text on screen.
        std::unique_ptr<Font> font{Font::create("resource/VCR_OSD_MONO_1.001.ttf")};

        /// We only want one instance of `Game`, we use this bool to track whether an instance was already created.
        static bool isInitialised;

         /// Create a new game instance.
         /// @param window_ The window to display the game on.
         /// @param tileMap_ The game map made up of square tiles.
        Game(std::unique_ptr<Window> window_, std::unique_ptr<TileMap> tileMap_);

    public:
        /// Create a new game instance.
        /// @param windowSize The width and height of the window to display the game in pixels.
        /// @return A game instance.
        static Game create(Size<int> windowSize);

        Game(Game&) = delete;
        Game(Game&&) = delete;

         /// Update the game by one step.
         /// @param deltaTime the size of the step to take in term of time (seconds).
        void update(float deltaTime);

        /// Render the game to the screen.
        void render() const;

        /// Run the main game loop (this call blocks).
        void run();
    };
} // namespace EconSimPlusPlus

#endif // GAME_H
