

#ifndef LIBTILEENGINE_TILEENGINE_GLYPH_HPP
#define LIBTILEENGINE_TILEENGINE_GLYPH_HPP

#include "glm/vec2.hpp"

namespace TileEngine {
    /// Represents a single character in a TrueType font.
    struct Glyph {
        /// The character this glyph represents.
        const unsigned char character;
        /// The width and height of the character.
        const glm::vec2 size;
        /// The horizontal and vertical offset to sit letters on the baseline.
        const glm::vec2 bearing;
        /// The spacing between this character and other characters.
        const float advance;

        /// Create a new Glyph.
        /// @param character_ The character this glyph represents.
        /// @param size_ The width and height of the character.
        /// @param bearing_ The horizontal and vertical offset to sit letters on the baseline.
        /// @param advance_ The spacing between this character and other characters.
        Glyph(unsigned char character_, glm::vec2 size_, glm::vec2 bearing_, float advance_);

        Glyph(Glyph&) = delete;
        Glyph(Glyph&&) = delete;
    };
} // namespace TileEngine


#endif // LIBTILEENGINE_TILEENGINE_GLYPH_HPP
