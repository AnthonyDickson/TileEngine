
#include <format>

#include "stb_image.h"

#include <EconSimPlusPlus/Image.hpp>

namespace EconSimPlusPlus::Image {
    Image create(const std::string& imagePath) {
        int width{};
        int height{};
        int channelCount{};
        stbi_set_flip_vertically_on_load(true);
        const auto byteBuffer{stbi_load(imagePath.c_str(), &width, &height, &channelCount, 0)};

        if (byteBuffer == nullptr) {
            throw std::runtime_error(
                std::format("Texture failed to load image from {0}: {1}", imagePath, stbi_failure_reason()));
        }

        const std::vector imageData(byteBuffer, byteBuffer + width * height * channelCount);

        stbi_image_free(byteBuffer);

        return {imageData, glm::ivec2{width, height}, channelCount, imagePath};
    }
} // namespace EconSimPlusPlus
