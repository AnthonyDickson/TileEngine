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
#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {
    /// Handles loading and accessing a textured-based tile map.
    class TileMap {
        const std::unique_ptr<Texture> texture;
        /// The size (width and height) of a single tile in pixels.
        const glm::vec2 tileSize;
        /// The size (width, height) of the tile sheet in tiles.
        const Size<int> sheetSize;
        /// The size (width, height) of the tile map in tiles.
        const Size<int> mapSize;
        /// The tiles of the tile map.
        const std::vector<int> tiles;
        /// Shader to render textured tiles.
        const Shader shader{"resource/shader/tile.vert", "resource/shader/tile.frag"};

        /// The vertex array for the tile quad.
        const VertexArray vao{};
        /// The vertex buffer for the tile quad.
        VertexBuffer vbo{};
        /// The UV corners for each tile.
        const std::vector<glm::vec2> textureCoordinates;

    public:
        /// @param texture_ The tile sheet texture.
        /// @param tileSize_ The size (width, height) of a single tile in pixels.
        /// @param mapSize_ The size (width, height) of the tile map in tiles.
        /// @param tiles_ The tiles in the tile map by integer ID. Zero indicates an empty tile.
        TileMap(std::unique_ptr<Texture> texture_, glm::vec2 tileSize_, Size<int> mapSize_,
                const std::vector<int>& tiles_);

        /// Construct a `TileMap` object from a YAML file.
        /// @param yamlPath The path to a YAML formatted tile map document.
        /// @return A `TileMap` pointer.
        static std::unique_ptr<TileMap> create(const std::string& yamlPath);

        /// Draw the tile map on screen.
        /// @param camera The camera to render the tile map with.
        void render(const Camera& camera) const;

    private:
        /// Generate the texture coordinates for a tile sheet.
        /// @note The UV coordinates are returned as a flat array, however each tile's coordinates take up 4 consecutive
        /// elements.
        /// @param sheetSize The width and height of the tile sheet in tiles.
        /// @return The UV coordinates for each tile (four elements each).
        static std::vector<glm::vec2> generateTextureCoordinates(Size<int> sheetSize);

        /// Bounds of a tile grid.
        struct GridBounds {
            int rowStart;
            int rowEnd;
            int colStart;
            int colEnd;
        };

        /// Find the area of the tile map visible to a camera.
        /// @param camera The camera to use for calculating visible tile maps.
        /// @return The visible area of the tile map.
        [[nodiscard]] GridBounds calculateVisibleGridBounds(const Camera& camera) const {
            const auto cameraPosition{camera.getPosition()};
            const glm::vec2 cameraPosition2D{cameraPosition.x, cameraPosition.y};
            const glm::vec2 viewport{camera.getViewportSize()};
            const glm::vec2 mapSizeVec{mapSize.width, mapSize.height};

            const auto viewBottomLeft{cameraPosition2D - viewport / 2.0f};
            const auto viewTopRight{cameraPosition2D + viewport / 2.0f};
            const auto gridOffsetMin{viewBottomLeft / tileSize};
            const auto gridOffsetMax{viewTopRight / tileSize};
            const glm::vec2 gridCoordinatesMin{gridOffsetMin + mapSizeVec / 2.0f};
            const glm::vec2 gridCoordinatesMax{gridOffsetMax + mapSizeVec / 2.0f};

            const int rowStart = std::max(0, static_cast<int>(gridCoordinatesMin.y));
            const int rowEnd = std::min(static_cast<int>(gridCoordinatesMax.y) + 1, mapSize.height);

            const int colStart = std::max(0, static_cast<int>(gridCoordinatesMin.x));
            const int colEnd = std::min(static_cast<int>(gridCoordinatesMax.x) + 1, mapSize.width);

            return {rowStart, rowEnd, colStart, colEnd};
        }
    };
} // namespace EconSimPlusPlus

#endif // TILEMAP_H
