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
// Created by Anthony Dickson on 03/06/2024.
//

#include "TileSheet.hpp"

namespace EconSimPlusPlus {
    namespace {
        glm::vec2 calculateSheetSize(const Texture* texture, const glm::vec2 tileSize) {
            return static_cast<glm::vec2>(texture->resolution()) / tileSize;
        }

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

    TileSheet::TileSheet(std::unique_ptr<Texture> texture, const glm::vec2 tileSize) :
        m_texture(std::move(texture)), m_tileSize(tileSize),
        m_sheetSize(calculateSheetSize(m_texture.get(), m_tileSize)), m_textureCoordinateStride(1.0f / m_sheetSize),
        m_textureCoordinates(generateTextureCoordinates(m_sheetSize)) {
    }

    glm::vec2 TileSheet::tileSize() const {
        return m_tileSize;
    }

    glm::vec2 TileSheet::textureCoordinateStride() const {
        return m_textureCoordinateStride;
    }

    glm::vec2 TileSheet::textureCoordinates(const int tileID) const {
        return m_textureCoordinates.at(tileID - 1);
    }

    int TileSheet::tileCount() const {
        return static_cast<int>(m_sheetSize.x * m_sheetSize.y);
    }

    glm::vec2 TileSheet::sheetSize() const {
        return m_sheetSize;
    }

    std::string TileSheet::texturePath() const {
        return m_texture->path();
    }

    void TileSheet::bind() const {
        m_texture->bind();
    }
} // namespace EconSimPlusPlus
