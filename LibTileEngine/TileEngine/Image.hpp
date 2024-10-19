

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <vector>

#include "glm/vec2.hpp"

namespace TileEngine::Image {
    struct Image {
        /// The raw byte data associated with the image.
        const std::vector<uint8_t> bytes;
        /// The size (width, height) of the texture in pixels.
        const glm::ivec2 resolution;
        /// The number of channels in the image data.
        ///
        /// Common channel counts:
        /// 1 = black and white,
        /// 3 = RGB,
        /// 4 = RGBA.
        const int channels;
        /// Where the texture was loaded from.
        const std::string path;
    };

    /// Load an image from a file path.
    /// @param imagePath The path to an image.
    Image create(const std::string& imagePath);
} // namespace TileEngine

#endif // IMAGE_HPP
