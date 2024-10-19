

#include <EconSimPlusPlus/Glyph.hpp>

namespace EconSimPlusPlus {
    Glyph::Glyph(const unsigned char character_, const glm::vec2 size_, const glm::vec2 bearing_,
                 const float advance_) : character(character_), size(size_), bearing(bearing_), advance(advance_) {
    }

} // namespace EconSimPlusPlus
