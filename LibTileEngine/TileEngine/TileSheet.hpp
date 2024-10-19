

#ifndef LIBTILEENGINE_TILEENGINE_TILESHEET_HPP
#define LIBTILEENGINE_TILEENGINE_TILESHEET_HPP

#include <memory>
#include <vector>

#include <glm/vec2.hpp>

#include <TileEngine/Texture.hpp>

namespace TileEngine {

    /// The texture containing tiles for a tile map and info on the tiles (e.g., size, texture coordinates).
    class TileSheet {
    public:
        /// Create a tile sheet.
        /// @param texture A texture containing a regular grid of tiles.
        /// @param tileSize The width and height of a tile in pixels.
        TileSheet(std::unique_ptr<Texture> texture, glm::vec2 tileSize);

        /// Get the dimensions of tiles in this tile sheet.
        /// @return The width and height in pixels.
        [[nodiscard]] glm::vec2 tileSize() const;

        /// Get the tile size in texture coordinates.
        /// @return Tile height and width in texture coordinates.
        [[nodiscard]] glm::vec2 textureCoordinateStride() const;

        /// Get the texture coordindates for a given tile.
        /// @param tileID The ID of a tile. Note that the IDs correspond to the indices calculated as
        /// `1 + row * width + col` using the tile sheet dimensions.
        /// @return The texture coordinates for the given tile.
        [[nodiscard]] glm::vec2 textureCoordinates(int tileID) const;

        /// Get the number of tiles in the tile sheet.
        [[nodiscard]] int tileCount() const;

        /// Get the tile sheet dimensions.
        /// @return The width and height measured in tiles.
        [[nodiscard]] glm::vec2 sheetSize() const;

        /// Get the path to the image file used to create the underlying texture.
        [[nodiscard]] std::string texturePath() const;

        /// Bind the tile sheet texture for rendering.
        void bind() const;

    private:
        /// The texture containing a regular grid of tiles.
        const std::unique_ptr<Texture> m_texture;
        /// The width and height of a tile in pixels.
        const glm::vec2 m_tileSize;
        /// The width and height of the tile sheet in tiles.
        const glm::vec2 m_sheetSize;
        /// The tile size in texture coordinates.
        const glm::vec2 m_textureCoordinateStride;
        /// The UV corners for each tile.
        const std::vector<glm::vec2> m_textureCoordinates;
    };

} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_TILESHEET_HPP
