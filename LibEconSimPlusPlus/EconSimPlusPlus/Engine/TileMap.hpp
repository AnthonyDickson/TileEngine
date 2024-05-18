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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_TILEMAP_H
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_TILEMAP_H

#include <EconSimPlusPlus/Engine/Camera.hpp>
#include <EconSimPlusPlus/Engine/GameObject.hpp>
#include <EconSimPlusPlus/Engine/Shader.hpp>
#include <EconSimPlusPlus/Engine/Texture.hpp>
#include <EconSimPlusPlus/Engine/VertexArray.hpp>
#include <EconSimPlusPlus/Engine/VertexBuffer.hpp>

namespace EconSimPlusPlus::Engine {
    /// Handles loading and accessing a textured-based tile map.
    class TileMap final : public GameObject {

    public:
        /// @param texture The tile sheet texture.
        /// @param tileSize The size (width, height) of a single tile in pixels.
        /// @param mapSize The size (width, height) of the tile map in tiles.
        /// @param tiles The tiles in the tile map by integer ID. Zero indicates an empty tile.
        TileMap(std::unique_ptr<Texture> texture, glm::vec2 tileSize, glm::ivec2 mapSize,
                const std::vector<int>& tiles);

        /// Construct a `TileMap` object from a YAML file.
        /// @param yamlPath The path to a YAML formatted tile map document.
        /// @return A `TileMap` pointer.
        static std::unique_ptr<TileMap> create(const std::string& yamlPath);

        /// The size (width, height) of the tile map in tiles.
        [[nodiscard]] glm::ivec2 mapSize() const;

        /// The size (width and height) of a single tile in pixels.
        [[nodiscard]] glm::vec2 tileSize() const;

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

        /// The tile sheet texture.
        const std::unique_ptr<Texture> m_texture;
        /// The size (width and height) of a single tile in pixels.
        const glm::vec2 m_tileSize;
        /// The size (width, height) of the tile sheet texture in tiles.
        const glm::ivec2 m_sheetSize;
        /// The size (width, height) of the tile map in tiles.
        const glm::ivec2 m_mapSize;
        /// The tiles of the tile map.
        const std::vector<int> m_tiles;

        /// Shader to render textured tiles.
        const Shader m_shader{Shader::create("resource/shader/tile.vert", "resource/shader/tile.frag")};
        /// The vertex array for the tile quad.
        const VertexArray m_vao{};
        /// The vertex buffer for the tile quad.
        VertexBuffer m_vbo{};
        /// The UV corners for each tile.
        const std::vector<glm::vec2> m_textureCoordinates;
    };
} // namespace EconSimPlusPlus::Engine

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_TILEMAP_H
