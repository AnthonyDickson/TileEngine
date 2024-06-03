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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TILEMAP_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TILEMAP_HPP

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/GameObject.hpp>
#include <EconSimPlusPlus/GridLines.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/TileSheet.hpp>

namespace EconSimPlusPlus {
    /// Handles loading and accessing a textured-based tile map.
    class TileMap final : public GameObject {

    public:
        /// Construct a `TileMap` object from a YAML file.
        /// @param yamlPath The path to a YAML formatted tile map document.
        /// @return A `TileMap` pointer.
        static std::unique_ptr<TileMap> create(const std::string& yamlPath);

        /// @param tileSheet The tile sheet.
        /// @param mapSize The size (width, height) of the tile map in tiles.
        /// @param tiles The tiles in the tile map by integer ID. Zero indicates an empty tile.
        TileMap(std::unique_ptr<TileSheet> tileSheet, glm::ivec2 mapSize, const std::vector<int>& tiles);

        /// The size (width, height) of the tile map in tiles.
        [[nodiscard]] glm::ivec2 mapSize() const;

        /// The size (width and height) of a single tile in pixels.
        [[nodiscard]] glm::vec2 tileSize() const;

        /// Add grid lines over the tile map.
        void enableGridLines();

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;

        /// Draw the tile map on screen.
        /// @param camera The camera to render the tile map with.
        void render(const Camera& camera) const override;

    private:
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
        [[nodiscard]] GridBounds calculateVisibleGridBounds(const Camera& camera) const;

        /// The tile sheet.
        const std::unique_ptr<TileSheet> m_tileSheet;
        /// The size (width, height) of the tile map in tiles.
        const glm::ivec2 m_mapSize;
        /// The tiles of the tile map.
        const std::vector<int> m_tiles;

        /// Shader to render textured tiles.
        const Shader m_shader{Shader::create("resource/shader/tile.vert", "resource/shader/tile.frag")};
        /// The tile geometry.
        const Quad m_quad{};

        /// Optional grid lines to draw over the tile map.
        std::optional<GridLines> m_gridLines{};
    };
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TILEMAP_HPP
