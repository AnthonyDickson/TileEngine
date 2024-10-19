
#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ANCHOR_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ANCHOR_HPP

#include "glm/vec2.hpp"

namespace EconSimPlusPlus {
    /// The point on an object that its position refers to.
    enum class Anchor { bottomLeft, bottomRight, topLeft, topRight, center };

    /// Calculate the positional offset for the given object size and anchor that gives the top left corner of an object
    /// when added to its raw position.
    /// @param size The dimensions (width, height) of the object.
    /// @param anchor The point on the object to use as the origin.
    /// @param verticalBaseline A vertical offset to ensure that the correct point on the y-axis is used as the origin.
    /// Important if the object is using world coordinates where +y points up.
    /// @return A 2D offset that, when added to the raw position, gives the top left corner of an object.
    /// @note Assumes screen space coordinates, i.e., +y points down.
    [[nodiscard]] glm::vec2 calculateAnchorOffset(glm::vec2 size, Anchor anchor, float verticalBaseline = 0.0f);
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ANCHOR_HPP
