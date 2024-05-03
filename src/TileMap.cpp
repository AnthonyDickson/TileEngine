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

    TileMap::TileMap(std::unique_ptr<Texture> texture_, const glm::vec2 tileSize_, const Size<int> mapSize_,
                     const std::vector<int>& tiles_) :
        texture(std::move(texture_)), tileSize(tileSize_),
        sheetSize{texture->resolution.x / static_cast<int>(tileSize_.x),
                  texture->resolution.y / static_cast<int>(tileSize_.y)},
        mapSize(mapSize_), tiles(tiles_), textureCoordinates(generateTextureCoordinates(sheetSize)) {
        const std::vector vertexData{0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};
        vao.bind();
        vbo.loadData(vertexData, {2});
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
        const Size<int> tileMapSize{tileMapNode["width"].as<int>(), tileMapNode["height"].as<int>()};
        const auto tiles{tileMapNode["tiles"].as<std::vector<int>>()};

        return std::make_unique<TileMap>(std::move(texture), tileSize, tileMapSize, tiles);
    }

    void TileMap::render(const Camera& camera) const {
        const auto [rowStart, rowEnd, colStart, colEnd]{calculateVisibleGridBounds(camera)};
        const glm::vec2 textureCoordStride{1.0f / static_cast<float>(sheetSize.width),
                                           1.0f / static_cast<float>(sheetSize.height)};

        shader.bind();
        shader.setUniform("projectionViewMatrix", camera.getPerspectiveMatrix() * camera.getViewMatrix());
        shader.setUniform("tileSize", textureCoordStride);
        texture->bind();
        vao.bind();
        vbo.bind();

        constexpr glm::mat4 identity{1.0f};
        const auto scale{glm::scale(identity, glm::vec3{tileSize.x, tileSize.y, 0.0})};
        std::vector transforms(shader.maxInstances, identity);
        std::vector textureCoordinatesInstanced(shader.maxInstances, glm::vec2(0.f));
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
                transforms[tileIndex] = glm::translate(
                    scale,
                    glm::vec3{(static_cast<float>(col) - static_cast<float>(mapSize.width) / 2.0f),
                              (static_cast<float>(row) - static_cast<float>(mapSize.height) / 2.0f), 0.0});
                const auto tileID{tiles.at(row * mapSize.width + col)};
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
