

#include <EconSimPlusPlus//TileSheet.hpp>

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
