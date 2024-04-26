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

#ifndef TILEMAP_H
#define TILEMAP_H

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/Size.hpp>
#include <EconSimPlusPlus/Texture.hpp>
#include <EconSimPlusPlus/TileTypes.hpp>

namespace EconSimPlusPlus {
    /** Handles loading and accessing a textured-based tile map. */
    class TileMap {
    private:
        const std::unique_ptr<Texture> texture;
        /// The size (width and height) of a single tile in pixels.
        const glm::vec2 tileSize;
        /// The size (width, height) of the tile sheet in tiles.
        const Size<int> sheetSize;
        /// The size (width, height) of the tile map in tiles.
        const Size<int> mapSize;
        /// The tiles of the tile map.
        const std::vector<int> tiles;
        /// The info on the tile types.
        const std::unique_ptr<TileTypes> tileTypes;
        /// Shader to render textured tiles.
        const Shader shader{"resource/shader/tile.vert", "resource/shader/tile.frag"};

    public:
        /**
         * @param texture_ The tile sheet texture.
         * @param tileSize_ The size (width, height) of a single tile in pixels.
         * @param mapSize_ The size (width, height) of the tile map in tiles.
         * @param tiles_ The tiles in the tile map by integer ID. Zero indicates an empty tile.
         */
        TileMap(std::unique_ptr<Texture> texture_, glm::vec2 tileSize_, Size<int> mapSize_,
                const std::vector<int>& tiles_);

        /**
         * Construct a `TileMap` object from a YAML file.
         * @param yamlPath The path to a YAML formatted tile map document.
         * @return A `TileMap` pointer.
         */
        static std::unique_ptr<TileMap> create(const std::string& yamlPath);

        /**
         * Draw the tile map on screen.
         * @param camera The camera to render the tile map with.
         */
        void render(const Camera& camera) const;
    };
} // namespace EconSimPlusPlus

#endif // TILEMAP_H
