#ifndef GAME_TILEENGINE_GAME_HPP
#define GAME_TILEENGINE_GAME_HPP

#include <TileEngine/Font.hpp>
#include <TileEngine/GridLines.hpp>
#include <TileEngine/TileMap.hpp>
#include <TileEngine/Window.hpp>

namespace TileEngine {
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
        void addObject(Object* object);

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
        Game(std::unique_ptr<Window> window, std::unique_ptr<TileMap> tileMap);

        /// We only want one instance of `Game`, we use this bool to track whether an instance was already created.
        static bool m_isInitialised;

        /// The window we use to display the game.
        std::unique_ptr<Window> m_window;
        /// The game 'map'.
        std::unique_ptr<TileMap> m_tileMap;
        /// A list of all game objects.
        std::vector<Object*> objects{};
        Graphics m_graphics;
        Graphics m_guiGraphics;
    };
} // namespace TileEngine

#endif // GAME_TILEENGINE_GAME_HPP
