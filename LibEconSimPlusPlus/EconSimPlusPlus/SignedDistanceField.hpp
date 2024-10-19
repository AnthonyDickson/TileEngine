

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_SIGNEDDISTANCEFIELD_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_SIGNEDDISTANCEFIELD_HPP

#include <vector>

#include <glm/vec2.hpp>

namespace EconSimPlusPlus::SignedDistanceField {
    /// Create a signed distance field (SDF) from a binary image.
    /// @param bitmap A black and white image where white pixels denote regions inside an object and black
    /// pixels regions outside an object.
    /// @param bitmapSize The width and height of the bitmap in pixels.
    /// @param paddedSize The desired width and height of the padded bitmap used for creating the SDF.
    /// @param outputSize The width and height in pixels of the output SDF image.
    /// @param spread A factor to divide the distance by. Larger values allows a larger range of values to be
    /// captured without being clipped.
    /// @return An 8-bit signed distance field (128.0f = 0).
    std::vector<std::uint8_t> createSDF(const std::uint8_t* bitmap, glm::ivec2 bitmapSize, glm::ivec2 paddedSize,
                                        glm::ivec2 outputSize, float spread = 16.0f);
} // namespace EconSimPlusPlus::SignedDistanceField

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_SIGNEDDISTANCEFIELD_HPP
