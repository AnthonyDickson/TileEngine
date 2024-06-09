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
// Created by Anthony Dickson on 22/04/2024.
//

#include <format>
#include <iostream>
#include <utility>

#include "glm/ext/matrix_transform.hpp"
#include "yaml-cpp/yaml.h"

#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/TileMap.hpp>

namespace EconSimPlusPlus {

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

        glm::mat4 transform{glm::scale(glm::mat4(1.0f), {m_tileSheet->tileSize(), 1.0f})};
        transform = glm::translate(transform, glm::vec3{-static_cast<glm::vec2>(mapSize) / 2.0f, 0.0f});
        setTransform(transform);
        setSize(m_tileSheet->tileSize() * static_cast<glm::vec2>(mapSize));
    }

    glm::ivec2 TileMap::mapSize() const {
        return m_mapSize;
    }

    glm::vec2 TileMap::tileSize() const {
        return m_tileSheet->tileSize();
    }

    void TileMap::addClickListener(const std::function<void(glm::ivec2)>& callback) {
        m_clickListeners.push_back(callback);
    }

    void TileMap::enableGridLines() {
        m_gridLines.emplace(mapSize(), tileSize());
        m_gridLines->setPosition(position());
    }

    void TileMap::setPosition(const glm::vec2 position) {
        GameObject::setPosition(position);

        if (m_gridLines.has_value()) {
            m_gridLines->setPosition(position);
        }
    }

    void TileMap::update(const float deltaTime, const InputState& inputState, const Camera& camera) {
        // ReSharper disable once CppTooWideScopeInitStatement
        const glm::vec2 cursorPos{screenToWorldCoordinates(inputState.getMousePosition(), camera)};

        if (inputState.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) and contains(cursorPos)) {
            glm::vec2 gridPos{(cursorPos - position()) / m_tileSheet->tileSize()};
            // The above calculates the grid position w/ the bottom left as the origin.
            // This results in the y-coordinates being in reverse order compared to the underlying array.
            // Therefore, we need to flip the y-coordinate here to ensure that the grid coordinates can be used to
            // recover the correct tileID.
            gridPos.y = static_cast<float>(mapSize().y) - gridPos.y;

            for (const auto& callback : m_clickListeners) {
                callback(gridPos);
            }
        }

        if (m_gridLines.has_value()) {
            m_gridLines->update(deltaTime, inputState, camera);
        }
    }

    void TileMap::render(const Camera& camera) const {
        const auto [rowStart, rowEnd, colStart, colEnd]{calculateVisibleGridBounds(camera)};

        m_shader.bind();
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * camera.viewMatrix());
        m_shader.setUniform("tileSize", m_tileSheet->textureCoordinateStride());
        m_tileSheet->bind();

        std::vector<glm::mat4> transforms(m_shader.maxInstances());
        std::vector<glm::vec2> textureCoordinatesInstanced(m_shader.maxInstances());
        int tileIndex{0};

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
                transforms[tileIndex] =
                    glm::translate(transform(), glm::vec3{static_cast<float>(col), static_cast<float>(row), 0.0f});
                const int tileID{m_tiles.at(row * m_mapSize.x + col)};
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
            m_gridLines->render(camera);
        }
    }

    TileMap::GridBounds TileMap::calculateVisibleGridBounds(const Camera& camera) const {
        const auto [bottomLeft, topRight]{camera.viewport()};

        const glm::vec2 gridCoordinatesMin{(bottomLeft - position()) / m_tileSheet->tileSize()};
        const glm::vec2 gridCoordinatesMax{(topRight - position()) / m_tileSheet->tileSize()};

        // This padding ensures that partially visible tiles at the edge of the screen are drawn to stop them 'suddenly
        // appearing' only once they are fully in view.
        constexpr int padding{1};

        const int rowStart = std::max(0, static_cast<int>(gridCoordinatesMin.y));
        const int rowEnd = std::min(static_cast<int>(gridCoordinatesMax.y) + padding, m_mapSize.y);

        const int colStart = std::max(0, static_cast<int>(gridCoordinatesMin.x));
        const int colEnd = std::min(static_cast<int>(gridCoordinatesMax.x) + padding, m_mapSize.x);

        return {rowStart, rowEnd, colStart, colEnd};
    }
} // namespace EconSimPlusPlus
