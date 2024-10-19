

#ifndef LIBTILEENGINE_TILEENGINE_VIEWPORT_HPP
#define LIBTILEENGINE_TILEENGINE_VIEWPORT_HPP

#include <glm/vec2.hpp>

namespace TileEngine {
    /// The area visible from a camera or window, defined by its extents.
    struct Viewport {
        /// The bottom left corner of the viewport in world space.
        glm::vec2 bottomLeft;
        /// The top right corner of the viewport in world space.
        glm::vec2 topRight;
    };
} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_VIEWPORT_HPP
