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

#include "KeyboardInput.h"
#include "TileGrid.h"
#include "TileGridView.h"
#include "Window.h"

class Game {
    /** This is the main class for the program. Wraps up and coordinates everything. */
private:
    /** The window we use to display the game. */
    std::unique_ptr<Window> window;
    /** Keeps track of state of each standard keyboard key. */
    KeyboardInput keyboardState{};

    /** The game 'map'. */
    std::shared_ptr<TileGrid> tileGrid;
    /** The view into the tile grid that is shown to the player, basically a camera. */
    std::shared_ptr<TileGridView> tileGridView;
    /** The render camera. */
    Camera camera;

    /** We only want one instance of `Game`, we use this bool to track whether an instance was already created. */
    static bool isInitialised;

    /**
     * Create a new game instance.
     * @param window_ The window to display the game on.
     * @param tileGrid_ The tile grid to use as the 'map'.
     * @param tileGridView_ The view of a tile grid to show the user.
     */
    Game(std::unique_ptr<Window> window_, std::shared_ptr<TileGrid> tileGrid_, std::shared_ptr<TileGridView> tileGridView_);

public:
    static Game create(Size<int> windowSize, Size<int> tileGridSize, int tileSize);

    /** Update the game by one step. */
    void update();
    /** Render the game to the screen. */
    void render() const;

    /** Run the main game loop (this call blocks). */
    void run();
};


#endif //GAME_H
