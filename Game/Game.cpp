#include <thread>
#include <utility>

#include "glm/ext/matrix_transform.hpp"

#include <TileEngine/FrameTimer.hpp>
#include <TileEngine/Text.hpp>
#include "Game.hpp"

namespace TileEngine {

    bool Game::m_isInitialised = false;

    Game::Game(std::unique_ptr<Window> window, std::unique_ptr<TileMap> tileMap) :
        m_window(std::move(window)), m_tileMap(std::move(tileMap)),
        m_graphics(Graphics{
            .camera = Camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                             {0.0f, 0.0f, 100.0f}},
        }),
        m_guiGraphics(Graphics{
            .camera = Camera{{static_cast<float>(m_window->width()), static_cast<float>(m_window->height())},
                             {0.0f, 0.0f, 100.0f}},
        }) {
        assert(!m_isInitialised && "Cannot have more than one instance of `Game`.");
        m_isInitialised = true;
    }

    Game Game::create(glm::ivec2 windowSize) {
        auto window{std::make_unique<Window>(windowSize.x, windowSize.y, "TileEngine")};
        std::unique_ptr tileMap{TileMap::create("resource/terrain.yaml")};

        return {std::move(window), std::move(tileMap)};
    }

    void Game::addObject(Object* object) {
        objects.push_back(object);
    }

    void Game::update(const float deltaTime) {
        if (m_window->hasWindowSizeChanged()) {
            m_graphics.camera.onWindowResize({static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
            m_guiGraphics.camera.onWindowResize({static_cast<float>(m_window->width()), static_cast<float>(m_window->height())});
        }

        const InputState input{m_window->inputState()};
        m_graphics.camera.update(deltaTime, input);

        for (const auto& object : objects) {
            object->update(deltaTime, input, m_graphics.camera);
        }
        // TODO: Get tile map and grid lines to react to mouse over and mouse click
        // TODO: Propogate mouse click event to objects. Only send event to upper most layer object? Send cursor
        // position in both screen and world coordinates?
    }

    void Game::render() const {
        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);

        for (const auto& object : objects) {
            object->render(m_graphics);
        }
    }

    void Game::run() {
        constexpr int targetFramesPerSecond{60};
        constexpr std::chrono::milliseconds targetFrameTime{1000 / targetFramesPerSecond};
        constexpr float timeStep{1.0f / targetFramesPerSecond};

        std::chrono::time_point lastFrameTime{std::chrono::steady_clock::now()};

        FrameTimer updateTimer{};
        FrameTimer renderTimer{};

        Text frameTimeText{
            "", m_guiGraphics.font.get(),
            Font::Style{
                .color = {1.0f, 1.0f, 0.0f}, .size = 32.0f, .outlineSize = 0.3f, .outlineColor = {0.0f, 0.0f, 0.0f}}};
        frameTimeText.setAnchor(Anchor::topLeft);
        frameTimeText.setLayer(99.0f);

        // TODO: Move this to factory function?
        m_tileMap->setLayer(1.0f);
        m_tileMap->setAnchor(Anchor::center);
        m_tileMap->enableGridLines();
        objects.push_back(m_tileMap.get());

        while (true) {
            const std::chrono::time_point currentTime{std::chrono::steady_clock::now()};
            const std::chrono::duration deltaTime{currentTime - lastFrameTime};
            lastFrameTime = currentTime;

            if (deltaTime < targetFrameTime) {
                std::this_thread::sleep_for(targetFrameTime - deltaTime);
            }

            if (m_window->inputState().key(GLFW_KEY_ESCAPE) or m_window->shouldClose()) {
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
            const glm::vec2 position{-static_cast<float>(m_window->width()) / 2.0f,
                                     static_cast<float>(m_window->height()) / 2.0f};
            frameTimeText.setText(frameTimeSummary);
            frameTimeText.setPosition(position);
            frameTimeText.render(m_guiGraphics);

            m_window->postUpdate();
        }
    }
} // namespace TileEngine
