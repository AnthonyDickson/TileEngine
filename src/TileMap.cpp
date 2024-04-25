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

#include "TileMap.h"
#include "Shader.h"

TileMap::TileMap(std::shared_ptr<Texture> texture_, const Size<int> tileSize_, const Size<int> mapSize_,
                 const std::vector<int>& tiles_) :
    texture(std::move(texture_)), tileSize(tileSize_),
    sheetSize{texture->resolution.width / tileSize_.width, texture->resolution.height / tileSize_.height},
    mapSize(mapSize_), tiles(tiles_), tileTypes(TileTypes::create(sheetSize)) {
}

std::shared_ptr<TileMap> TileMap::create(const std::string& yamlPath) {
    const auto tileMapConfig{YAML::LoadFile(yamlPath)};
    const auto tileSheetNode{tileMapConfig["tile-sheet"]};

    const auto texturePath{tileSheetNode["path"].as<std::string>()};
    const auto texture{Texture::create(texturePath)};

    const auto tileSizeNode{tileSheetNode["tile-size"]};
    // ReSharper disable once CppTemplateArgumentsCanBeDeduced
    const Size<int> tileSize{tileSizeNode["width"].as<int>(), tileSizeNode["height"].as<int>()};

    const auto tileMapNode{tileMapConfig["tile-map"]};
    // ReSharper disable once CppTemplateArgumentsCanBeDeduced
    const Size<int> tileMapSize{tileMapNode["width"].as<int>(), tileMapNode["height"].as<int>()};
    const auto tiles{tileMapNode["tiles"].as<std::vector<int>>()};

    return std::make_shared<TileMap>(texture, tileSize, tileMapSize, tiles);
}

void TileMap::render(const Camera& camera) const {
    shader.bind();
    shader.setUniform("projectionViewMatrix", camera.getPerspectiveMatrix() * camera.getViewMatrix());
    texture->bind();

    // TODO: Change underlying Size types to glm::vec2.
    const auto cameraPosition{camera.getPosition()};
    const glm::vec2 cameraPosition2D{cameraPosition.x, cameraPosition.y};
    const glm::vec2 viewport{camera.getViewportSize()};
    const glm::vec2 tileSizeVec{tileSize.width, tileSize.height};
    const glm::vec2 mapSizeVec{mapSize.width, mapSize.height};

    const auto viewBottomLeft{cameraPosition2D - viewport / 2.0f};
    const auto viewTopRight{cameraPosition2D + viewport / 2.0f};
    const auto gridOffsetMin{viewBottomLeft / tileSizeVec};
    const auto gridOffsetMax{viewTopRight / tileSizeVec};
    const glm::vec2 gridCoordinatesMin{gridOffsetMin + mapSizeVec / 2.0f};
    const glm::vec2 gridCoordinatesMax{gridOffsetMax + mapSizeVec / 2.0f};

    const int rowStart = std::max(0, static_cast<int>(gridCoordinatesMin.y));
    const int rowEnd = std::min(static_cast<int>(gridCoordinatesMax.y) + 1, mapSize.height);

    const int colStart = std::max(0, static_cast<int>(gridCoordinatesMin.x));
    const int colEnd = std::min(static_cast<int>(gridCoordinatesMax.x) + 1, mapSize.width);

    constexpr glm::mat4 identity{1.0f};

    for (int row = rowStart; row < rowEnd; ++row) {
        for (int col = colStart; col < colEnd; ++col) {
            glm::mat4 model = glm::translate(identity,
                                             glm::vec3{(static_cast<float>(col) - mapSizeVec.x / 2.0f) * tileSizeVec.x,
                                                       (static_cast<float>(row) - mapSizeVec.y / 2.0f) * tileSizeVec.y, 0.0});
            model = glm::scale(model, glm::vec3{tileSize.width, tileSize.height, 0.0});

            shader.setUniform("model", model);
            const int tileID{tiles.at(row * mapSize.width + col)};
            tileTypes->render(tileID);
        }
    }
}
