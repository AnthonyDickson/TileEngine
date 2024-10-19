

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_VIEWPORT_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_VIEWPORT_HPP

#include <glm/vec2.hpp>

namespace EconSimPlusPlus {
    /// The area visible from a camera or window, defined by its extents.
    struct Viewport {
        /// The bottom left corner of the viewport in world space.
        glm::vec2 bottomLeft;
        /// The top right corner of the viewport in world space.
        glm::vec2 topRight;
    };
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_VIEWPORT_HPP
