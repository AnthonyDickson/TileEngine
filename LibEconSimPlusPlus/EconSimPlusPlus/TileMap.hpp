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
#include <EconSimPlusPlus/GridLines.hpp>
#include <EconSimPlusPlus/Object.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/TileSheet.hpp>
#include <functional>

namespace EconSimPlusPlus {
    /// Handles loading and accessing a textured-based tile map.
    class TileMap final : public Object {

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

        /// Set the map size.
        /// @param mapSize The new size (width, height) of the tile map in tiles.
        void setMapSize(glm::ivec2 mapSize);

        /// The size (width and height) of a single tile in pixels.
        [[nodiscard]] glm::vec2 tileSize() const;

        /// Get the tile ID at the given map coordinates.
        /// @note Tile IDs are one-based and zero is reserved to indicate an empty tile.
        /// @param gridCoordinates The coordinates (row and column) of the tile to set.
        /// @return the tile ID.
        [[nodiscard]] int tileID(glm::ivec2 gridCoordinates) const;

        /// Set the value of a given tile.
        /// @param gridCoordinates The coordinates (row and column) of the tile to set.
        /// @param tileID The value to set the tile to.
        void setTileID(glm::ivec2 gridCoordinates, int tileID);

        /// Get the tiles of the tile map as a flat list.
        /// @return A list of tile IDs.
        [[nodiscard]] std::vector<int> tiles() const;

        /// Get the path to the image file used to create the underlying texture.
        [[nodiscard]] std::string texturePath() const;

        /// Register a callback for when a tile is clicked.
        /// @param callback A function that takes a grid coordinate (glm::vec2) and tile ID (int) as an argument.
        void addClickListener(const std::function<void(glm::ivec2 gridCoordinates, int tileID)>& callback);

        /// Add grid lines over the tile map.
        void enableGridLines();

        void setPosition(glm::vec2 position) override;
        void setLayer(float layer) override;
        void setAnchor(Anchor anchor) override;

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Graphics& graphics) const override;

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
        glm::ivec2 m_mapSize;
        /// The tiles of the tile map.
        std::vector<int> m_tiles;

        /// Shader to render textured tiles.
        const Shader m_shader{Shader::create("resource/shader/tile.vert", "resource/shader/tile.frag")};
        /// The tile geometry.
        const Quad m_quad{};

        /// Optional grid lines to draw over the tile map.
        std::optional<GridLines> m_gridLines{};
        /// Functions to be called when a tile is clicked.
        std::vector<std::function<void(glm::ivec2 gridCoordinates, int tileID)>> m_clickListeners;
    };
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_TILEMAP_HPP
