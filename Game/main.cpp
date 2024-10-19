#include <iostream>

#include "Game.hpp"

int main() {
    try {
        auto game{TileEngine::Game::create({1920, 1080})};
        game.run();
    } catch (const std::exception &exception) {
        std::cout << "Program exited with unhandled exception: " << exception.what() << std::endl;
    }

    return 0;
}
