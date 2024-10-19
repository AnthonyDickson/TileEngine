

#include <format>
#include <iostream>
#include <utility>

#include "glm/ext/matrix_transform.hpp"
#include "yaml-cpp/yaml.h"

#include <TileEngine/Shader.hpp>
#include <TileEngine/TileMap.hpp>


namespace TileEngine {
    namespace {
        /// Resize a tile map.
        /// @param oldTiles The tiles in the old tile map.
        /// @param oldSize The width and height of the old tile map in tiles.
        /// @param newSize The width and height of the new tile map in tiles.
        /// @return The tiles in the new tile map, with newly added tiles being set to zero (empty).
        std::vector<int> resizeTileMap(const std::vector<int>& oldTiles, const glm::ivec2 oldSize,
                                       const glm::ivec2 newSize) {
            std::vector<int> newTiles(newSize.x * newSize.y);

            const int colCount{std::min(oldSize.x, newSize.x)};
            const int rowCount{std::min(oldSize.y, newSize.y)};

            for (int row = 0; row < rowCount; ++row) {
                for (int col = 0; col < colCount; ++col) {
                    const int oldIndex{row * oldSize.x + col};
                    const int newIndex{row * newSize.x + col};
                    newTiles.at(newIndex) = oldTiles.at(oldIndex);
                }
            }

            return newTiles;
        }
    }

    std::unique_ptr<TileMap> TileMap::create(const std::string& yamlPath) {
        const YAML::Node tileMapConfig{YAML::LoadFile(yamlPath)};
        const YAML::Node tileSheetNode{tileMapConfig["tile-sheet"]};

        const auto texturePath{tileSheetNode["path"].as<std::string>()};
        std::unique_ptr texture{Texture::create(texturePath)};

        const YAML::Node tileSizeNode{tileSheetNode["tile-size"]};
        // ReSharper disable once CppTemplateArgumentsCanBeDeduced
        const glm::vec2 tileSize{tileSizeNode["width"].as<int>(), tileSizeNode["height"].as<int>()};

        const YAML::Node tileMapNode{tileMapConfig["tile-map"]};
        // ReSharper disable once CppTemplateArgumentsCanBeDeduced
        const glm::ivec2 tileMapSize{tileMapNode["width"].as<int>(), tileMapNode["height"].as<int>()};
        const auto tiles{tileMapNode["tiles"].as<std::vector<int>>()};

        auto tileSheet{std::make_unique<TileSheet>(std::move(texture), tileSize)};

        return std::make_unique<TileMap>(std::move(tileSheet), tileMapSize, tiles);
    }

    TileMap::TileMap(std::unique_ptr<TileSheet> tileSheet, const glm::ivec2 mapSize, const std::vector<int>& tiles) :
        m_tileSheet(std::move(tileSheet)), m_mapSize(mapSize), m_tiles(tiles) {

        Object::setSize(tileSize() * static_cast<glm::vec2>(mapSize));

        addEventHandler([&](const Event event, const EventData& eventData) {
            if (event == Event::mouseClick) {
                const glm::vec2 gridPos{(eventData.mousePosition.value() - bottomLeft(*this)) /
                                        m_tileSheet->tileSize()};

                for (const auto& callback : m_clickListeners) {
                    callback(gridPos, tileID(gridPos));
                }
            }
        });
    }

    glm::ivec2 TileMap::mapSize() const {
        return m_mapSize;
    }

    void TileMap::setMapSize(const glm::ivec2 mapSize) {
        assert(glm::all(glm::greaterThan(mapSize, glm::ivec2{0})) && "Map size must be positive.");

        if (glm::all(glm::equal(mapSize, m_mapSize))) {
            return;
        }

        m_tiles = resizeTileMap(m_tiles, m_mapSize, mapSize);
        m_mapSize = mapSize;
        setSize(tileSize() * static_cast<glm::vec2>(mapSize));

        if (m_gridLines.has_value()) {
            enableGridLines();
        }
    }

    glm::vec2 TileMap::tileSize() const {
        return m_tileSheet->tileSize();
    }

    int TileMap::tileID(const glm::ivec2 gridCoordinates) const {
        return m_tiles.at(gridCoordinates.y * mapSize().x + gridCoordinates.x);
    }

    void TileMap::setTileID(const glm::ivec2 gridCoordinates, const int tileID) {
        m_tiles.at(gridCoordinates.y * mapSize().x + gridCoordinates.x) = tileID;
    }

    std::vector<int> TileMap::tiles() const {
        return m_tiles;
    }

    std::string TileMap::texturePath() const {
        return m_tileSheet->texturePath();
    }

    void TileMap::addClickListener(const std::function<void(glm::ivec2 gridCoordinate, int tileID)>& callback) {
        m_clickListeners.push_back(callback);
    }

    void TileMap::enableGridLines() {
        m_gridLines.emplace(mapSize(), tileSize());
        m_gridLines->setPosition(position());
        m_gridLines->setLayer(layer());
        m_gridLines->setAnchor(anchor());
    }

    void TileMap::setPosition(const glm::vec2 position) {
        Object::setPosition(position);

        if (m_gridLines.has_value()) {
            m_gridLines->setPosition(position);
        }
    }

    void TileMap::setLayer(const float layer) {
        Object::setLayer(layer);

        if (m_gridLines.has_value()) {
            m_gridLines->setLayer(layer);
        }
    }

    void TileMap::setAnchor(const Anchor anchor) {
        Object::setAnchor(anchor);

        if (m_gridLines.has_value()) {
            m_gridLines->setAnchor(anchor);
        }
    }

    void TileMap::update(const float deltaTime, const InputState& inputState, const Camera& camera) {
        if (m_gridLines.has_value()) {
            m_gridLines->update(deltaTime, inputState, camera);
        }
    }

    void TileMap::render(const Graphics& graphics) const {
        const auto [rowStart, rowEnd, colStart, colEnd]{calculateVisibleGridBounds(graphics.camera)};

        m_shader.bind();
        m_shader.setUniform("projectionViewMatrix", projectionViewMatrix(graphics.camera));
        m_shader.setUniform("tileSize", m_tileSheet->textureCoordinateStride());
        m_tileSheet->bind();

        std::vector<glm::mat4> transforms(m_shader.maxInstances());
        std::vector<glm::vec2> textureCoordinatesInstanced(m_shader.maxInstances());
        int tileIndex{0};
        const glm::mat4 tileTransform{glm::scale(glm::translate(glm::mat4{1.0f}, glm::vec3{bottomLeft(*this), layer()}),
                                                 glm::vec3{tileSize(), 1.0f})};

        auto renderFn = [&] {
            if (tileIndex == 0) {
                return;
            }

            glUniformMatrix4fv(m_shader.uniformLocation("transforms"), tileIndex, GL_FALSE, &transforms[0][0][0]);
            glUniform2fv(m_shader.uniformLocation("textureCoordinates"), tileIndex, &textureCoordinatesInstanced[0][0]);
            m_quad.render(tileIndex, GL_TRIANGLE_STRIP);
        };

        for (int row = rowStart; row < rowEnd; ++row) {
            for (int col = colStart; col < colEnd; ++col) {
                const glm::ivec2 gridCoordinates{col, row};
                const int tileID = this->tileID(gridCoordinates);

                if (tileID == 0) {
                    continue;
                }

                transforms[tileIndex] =
                    glm::translate(tileTransform, glm::vec3{static_cast<float>(col), static_cast<float>(row), 0.0f});
                textureCoordinatesInstanced[tileIndex] = m_tileSheet->textureCoordinates(tileID);

                ++tileIndex;

                if (tileIndex == m_shader.maxInstances()) {
                    renderFn();
                    tileIndex = 0;
                }
            }
        }

        renderFn();

        if (m_gridLines.has_value()) {
            m_gridLines->render(graphics);
        }
    }

    TileMap::GridBounds TileMap::calculateVisibleGridBounds(const Camera& camera) const {
        const auto [viewBottomLeft, viewTopRight]{camera.viewport()};
        const glm::vec2 bottomLeft{TileEngine::bottomLeft(*this)};

        const glm::vec2 gridCoordinatesMin{(viewBottomLeft - bottomLeft) / m_tileSheet->tileSize()};
        const glm::vec2 gridCoordinatesMax{(viewTopRight - bottomLeft) / m_tileSheet->tileSize()};

        // This padding ensures that partially visible tiles at the edge of the screen are drawn to stop them 'suddenly
        // appearing' only once they are fully in view.
        constexpr int padding{1};

        const int rowStart = std::max(0, static_cast<int>(gridCoordinatesMin.y));
        const int rowEnd = std::min(static_cast<int>(gridCoordinatesMax.y) + padding, m_mapSize.y);

        const int colStart = std::max(0, static_cast<int>(gridCoordinatesMin.x));
        const int colEnd = std::min(static_cast<int>(gridCoordinatesMax.x) + padding, m_mapSize.x);

        return {rowStart, rowEnd, colStart, colEnd};
    }

} // namespace TileEngine
