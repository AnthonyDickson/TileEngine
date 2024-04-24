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

#include "yaml-cpp/yaml.h"

#include "TileMap.h"

TileMap::TileMap(std::shared_ptr<Texture> texture_, const Size<int> tileSize_, const Size<int> mapSize_,
                 const std::vector<int>& tiles_) :
    texture(std::move(texture_)), tileSize(tileSize_),
    sheetSize{texture->resolution.width / tileSize_.width, texture->resolution.height / tileSize_.height},
    mapSize(mapSize_), tiles(tiles_) {
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
