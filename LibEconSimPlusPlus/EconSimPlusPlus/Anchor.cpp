
#include <EconSimPlusPlus/Anchor.hpp>

namespace EconSimPlusPlus {
    glm::vec2 calculateAnchorOffset(const glm::vec2 size, const Anchor anchor, const float verticalBaseline) {
        switch (anchor) {
        case Anchor::bottomLeft:
            return glm::vec2{0.0f, size.y - verticalBaseline};
        case Anchor::bottomRight:
            return glm::vec2{-size.x, size.y - verticalBaseline};
        case Anchor::topLeft:
            return glm::vec2{0.0f, 0.0f - verticalBaseline};
        case Anchor::topRight:
            return glm::vec2{-size.x, 0.0f - verticalBaseline};
        case Anchor::center:
            return {-size.x / 2.0f, size.y / 2.0f - verticalBaseline};
        default:
            return glm::vec2{0.0f};
        }
    }
} // namespace EconSimPlusPlus
