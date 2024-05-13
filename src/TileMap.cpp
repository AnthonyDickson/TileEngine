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

#include <utility>

#include "glm/ext/matrix_transform.hpp"
#include "yaml-cpp/yaml.h"

#include <EconSimPlusPlus/TileMap.hpp>

#include <EconSimPlusPlus/Shader.hpp>

namespace EconSimPlusPlus {

    TileMap::TileMap(std::unique_ptr<Texture> texture_, const glm::vec2 tileSize_, const glm::ivec2 mapSize_,
                     const std::vector<int>& tiles_) :
        texture(std::move(texture_)), tileSize(tileSize_),
        sheetSize{texture->resolution.x / static_cast<int>(tileSize_.x),
                  texture->resolution.y / static_cast<int>(tileSize_.y)},
        mapSize(mapSize_), tiles(tiles_), textureCoordinates(generateTextureCoordinates(sheetSize)) {
        const std::vector vertexData{0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        vao.bind();
        vbo.loadData(vertexData, {2});

        glm::mat4 transform{glm::scale(glm::mat4(1.0f), {tileSize, 1.0f})};
        transform = glm::translate(transform, glm::vec3{-static_cast<glm::vec2>(mapSize) / 2.0f, 0.0f});
        setTransform(transform);
        setSize({texture->resolution, 1.0f});
    }

    std::vector<glm::vec2> TileMap::generateTextureCoordinates(const Size<int> sheetSize) {
        const auto width{1.0f / static_cast<float>(sheetSize.width)};
        const auto height{1.0f / static_cast<float>(sheetSize.height)};

        std::vector<glm::vec2> textureCoordinates{};
        textureCoordinates.reserve(sheetSize.width * sheetSize.height);

        for (int row = 0; row < sheetSize.height; ++row) {
            for (int col = 0; col < sheetSize.width; ++col) {
                const auto u{static_cast<float>(col) * width};
                const auto v{static_cast<float>(row) * height};

                textureCoordinates.emplace_back(u, v);
            }
        }

        return textureCoordinates;
    }

    TileMap::GridBounds TileMap::calculateVisibleGridBounds(const Camera& camera) const {
        const auto [bottomLeft, topRight]{camera.viewport()};
        const glm::vec2 position{m_transform[3][0], m_transform[3][1]};

        const auto gridCoordinatesMin{(bottomLeft - position) / tileSize};
        const auto gridCoordinatesMax{(topRight - position) / tileSize};

        // This padding ensures that partially visible tiles at the edge of the screen are drawn to stop them 'suddenly
        // appearing' only once they are fully in view.
        constexpr int padding{1};

        const int rowStart = std::max(0, static_cast<int>(gridCoordinatesMin.y));
        const int rowEnd = std::min(static_cast<int>(gridCoordinatesMax.y) + padding, mapSize.y);

        const int colStart = std::max(0, static_cast<int>(gridCoordinatesMin.x));
        const int colEnd = std::min(static_cast<int>(gridCoordinatesMax.x) + padding, mapSize.x);

        return {rowStart, rowEnd, colStart, colEnd};
    }

    std::unique_ptr<TileMap> TileMap::create(const std::string& yamlPath) {
        const auto tileMapConfig{YAML::LoadFile(yamlPath)};
        const auto tileSheetNode{tileMapConfig["tile-sheet"]};

        const auto texturePath{tileSheetNode["path"].as<std::string>()};
        auto texture{Texture::create(texturePath)};

        const auto tileSizeNode{tileSheetNode["tile-size"]};
        // ReSharper disable once CppTemplateArgumentsCanBeDeduced
        const glm::vec2 tileSize{tileSizeNode["width"].as<int>(), tileSizeNode["height"].as<int>()};

        const auto tileMapNode{tileMapConfig["tile-map"]};
        // ReSharper disable once CppTemplateArgumentsCanBeDeduced
        const glm::ivec2 tileMapSize{tileMapNode["width"].as<int>(), tileMapNode["height"].as<int>()};
        const auto tiles{tileMapNode["tiles"].as<std::vector<int>>()};

        return std::make_unique<TileMap>(std::move(texture), tileSize, tileMapSize, tiles);
    }

    glm::ivec2 TileMap::getMapSize() const {
        return mapSize;
    }

    glm::vec2 TileMap::getTileSize() const {
        return tileSize;
    }

    void TileMap::render(const Camera& camera, const float z) const {
        const auto [rowStart, rowEnd, colStart, colEnd]{calculateVisibleGridBounds(camera)};
        const glm::vec2 textureCoordStride{1.0f / static_cast<float>(sheetSize.width),
                                           1.0f / static_cast<float>(sheetSize.height)};

        shader.bind();
        shader.setUniform("projectionViewMatrix", camera.getPerspectiveMatrix() * camera.getViewMatrix());
        shader.setUniform("tileSize", textureCoordStride);
        texture->bind();
        vao.bind();
        vbo.bind();

        std::vector<glm::mat4> transforms(shader.maxInstances);
        std::vector<glm::vec2> textureCoordinatesInstanced(shader.maxInstances);
        int tileIndex{0};

        auto renderFn = [&] {
            if (tileIndex == 0) {
                return;
            }

            glUniformMatrix4fv(shader.getUniformLocation("transforms"), tileIndex, GL_FALSE, &transforms[0][0][0]);
            glUniform2fv(shader.getUniformLocation("textureCoordinates"), tileIndex,
                         &textureCoordinatesInstanced[0][0]);
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tileIndex);
        };

        for (int row = rowStart; row < rowEnd; ++row) {
            for (int col = colStart; col < colEnd; ++col) {
                transforms[tileIndex] =
                    glm::translate(m_transform, glm::vec3{static_cast<float>(col), static_cast<float>(row), z});
                const auto tileID{tiles.at(row * mapSize.x + col)};
                textureCoordinatesInstanced[tileIndex] = textureCoordinates[tileID];

                ++tileIndex;

                if (tileIndex == shader.maxInstances) {
                    renderFn();
                    tileIndex = 0;
                }
            }
        }

        renderFn();
    }
} // namespace EconSimPlusPlus
