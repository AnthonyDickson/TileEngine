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
    namespace {
        /// Generate the texture coordinates for a tile sheet.
        /// @note The UV coordinates are returned as a flat array, however each tile's coordinates take up 4 consecutive
        /// elements.
        /// @param sheetSize The width and height of the tile sheet in tiles.
        /// @return The UV coordinates for each tile (four elements each).
        std::vector<glm::vec2> generateTextureCoordinates(const glm::ivec2 sheetSize) {
            const auto width{1.0f / static_cast<float>(sheetSize.x)};
            const auto height{1.0f / static_cast<float>(sheetSize.y)};

            std::vector<glm::vec2> textureCoordinates{};
            textureCoordinates.reserve(sheetSize.x * sheetSize.y);

            for (int row = 0; row < sheetSize.y; ++row) {
                for (int col = 0; col < sheetSize.x; ++col) {
                    const auto u{static_cast<float>(col) * width};
                    const auto v{static_cast<float>(row) * height};

                    textureCoordinates.emplace_back(u, v);
                }
            }

            return textureCoordinates;
        }
    } // namespace

    TileMap::TileMap(std::unique_ptr<Texture> texture, const glm::vec2 tileSize, const glm::ivec2 mapSize,
                     const std::vector<int>& tiles) :
        m_texture(std::move(texture)), m_tileSize(tileSize),
        m_sheetSize{m_texture->resolution() / static_cast<glm::ivec2>(tileSize)}, m_mapSize(mapSize), m_tiles(tiles),
        m_textureCoordinates(generateTextureCoordinates(m_sheetSize)) {
        const std::vector vertexData{0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        m_vao.bind();
        m_vbo.loadData(vertexData, {2});

        glm::mat4 transform{glm::scale(glm::mat4(1.0f), {tileSize, 1.0f})};
        transform = glm::translate(transform, glm::vec3{-static_cast<glm::vec2>(mapSize) / 2.0f, 0.0f});
        setTransform(transform);
        setSize({tileSize * static_cast<glm::vec2>(mapSize), 1.0f});
    }

    TileMap::GridBounds TileMap::calculateVisibleGridBounds(const Camera& camera) const {
        const auto [bottomLeft, topRight]{camera.viewport()};
        const glm::vec2 position{m_transform[3][0], m_transform[3][1]};

        const glm::vec2 gridCoordinatesMin{(bottomLeft - position) / m_tileSize};
        const glm::vec2 gridCoordinatesMax{(topRight - position) / m_tileSize};

        // This padding ensures that partially visible tiles at the edge of the screen are drawn to stop them 'suddenly
        // appearing' only once they are fully in view.
        constexpr int padding{1};

        const int rowStart = std::max(0, static_cast<int>(gridCoordinatesMin.y));
        const int rowEnd = std::min(static_cast<int>(gridCoordinatesMax.y) + padding, m_mapSize.y);

        const int colStart = std::max(0, static_cast<int>(gridCoordinatesMin.x));
        const int colEnd = std::min(static_cast<int>(gridCoordinatesMax.x) + padding, m_mapSize.x);

        return {rowStart, rowEnd, colStart, colEnd};
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

        return std::make_unique<TileMap>(std::move(texture), tileSize, tileMapSize, tiles);
    }

    glm::ivec2 TileMap::mapSize() const {
        return m_mapSize;
    }

    glm::vec2 TileMap::tileSize() const {
        return m_tileSize;
    }

    void TileMap::render(const Camera& camera, const float z) const {
        const auto [rowStart, rowEnd, colStart, colEnd]{calculateVisibleGridBounds(camera)};
        const glm::vec2 textureCoordStride{1.0f / static_cast<float>(m_sheetSize.x),
                                           1.0f / static_cast<float>(m_sheetSize.y)};

        m_shader.bind();
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * camera.viewMatrix());
        m_shader.setUniform("tileSize", textureCoordStride);
        m_texture->bind();
        m_vao.bind();
        m_vbo.bind();

        std::vector<glm::mat4> transforms(m_shader.maxInstances());
        std::vector<glm::vec2> textureCoordinatesInstanced(m_shader.maxInstances());
        int tileIndex{0};

        auto renderFn = [&] {
            if (tileIndex == 0) {
                return;
            }

            glUniformMatrix4fv(m_shader.uniformLocation("transforms"), tileIndex, GL_FALSE, &transforms[0][0][0]);
            glUniform2fv(m_shader.uniformLocation("textureCoordinates"), tileIndex, &textureCoordinatesInstanced[0][0]);
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tileIndex);
        };

        for (int row = rowStart; row < rowEnd; ++row) {
            for (int col = colStart; col < colEnd; ++col) {
                transforms[tileIndex] =
                    glm::translate(m_transform, glm::vec3{static_cast<float>(col), static_cast<float>(row), z});
                const int tileID{m_tiles.at(row * m_mapSize.x + col)};
                textureCoordinatesInstanced[tileIndex] = m_textureCoordinates[tileID];

                ++tileIndex;

                if (tileIndex == m_shader.maxInstances()) {
                    renderFn();
                    tileIndex = 0;
                }
            }
        }

        renderFn();
    }

    void TileMap::update(float, const InputState& inputState, const Camera& camera) {
        const glm::vec2 cursorPos{screenToWorldCoordinates(camera, inputState.getMousePosition())};

        if (contains(cursorPos) and inputState.getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
            std::cout << std::format("Mouse clicked over tile map at ({:.2f}, {:.2f}).\n", cursorPos.x, cursorPos.y);
        }
    }
} // namespace EconSimPlusPlus
