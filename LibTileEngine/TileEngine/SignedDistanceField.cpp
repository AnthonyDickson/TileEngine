

#include <algorithm>
#include <cmath>

#include <stb_image_resize2.h>

#include <TileEngine/SignedDistanceField.hpp>

namespace TileEngine {
    namespace {
        /// Pad an image with zeros.
        /// @param binaryImage An image buffer.
        /// @param inputSize The width and height of the image buffer in pixels.
        /// @param outputSize The desired width and height of the padded image buffer.
        /// @return The image buffer padded with zeros.
        template <typename PixelType>
        std::vector<PixelType> padImage(const PixelType* binaryImage, const glm::ivec2 inputSize,
                                        const glm::ivec2 outputSize) {
            glm::ivec2 padding{(outputSize - inputSize) / 2};
            padding.x = std::max(padding.x, 0);
            padding.y = std::max(padding.y, 0);

            std::vector<PixelType> paddedBitmap(outputSize.x * outputSize.y, 0);

            for (int row = 0; row < inputSize.y; ++row) {
                if (row + padding.y >= outputSize.y) {
                    break;
                }

                for (int col = 0; col < inputSize.x; ++col) {
                    if (col + padding.x >= outputSize.x) {
                        break;
                    }

                    paddedBitmap.at((row + padding.y) * outputSize.x + (col + padding.x)) =
                        binaryImage[row * inputSize.x + col];
                }
            }

            return paddedBitmap;
        }

        /// Generate a signed distance field from a binary image.
        /// @note Adapted from the paper:
        /// Grevera, George J. "The “dead reckoning” signed distance transform."
        /// Computer Vision and Image Understanding 95, no. 3 (2004): 317-333.
        /// @param buffer The single-channel image buffer containing a binary (black and white) image.
        /// @param bufferSize The width and height in pixels of the input image buffer.
        /// @return A single-channel image of the same resolution as the input.
        std::vector<float> createFloatSDF(const std::uint8_t* buffer, const glm::ivec2 bufferSize) {
            std::vector<float> sdf(bufferSize.x * bufferSize.y);
            std::vector<glm::ivec2> borderPoints(bufferSize.x * bufferSize.y);

            constexpr bool inside{true};
            constexpr bool outside{false};

            const auto contains = [&](const int x, const int y) {
                return 0 <= x and x < bufferSize.x and 0 <= y and y < bufferSize.y;
            };

            const auto I = [&](const int x, const int y) {
                if (not contains(x, y)) {
                    return outside;
                }

                return buffer[y * bufferSize.x + x] > 128 ? inside : outside;
            };

            constexpr float infinity = std::numeric_limits<float>::infinity();
            constexpr glm::ivec2 outOfBounds{-1, 1};

            const auto d = [&](const int x, const int y) {
                if (not contains(x, y)) {
                    return infinity;
                }

                return sdf.at(y * bufferSize.x + x);
            };

            const auto setd = [&](const int x, const int y, const float value) {
                if (not contains(x, y)) {
                    return;
                }

                sdf.at(y * bufferSize.x + x) = value;
            };

            const auto p = [&](const int x, const int y) {
                if (not contains(x, y)) {
                    return outOfBounds;
                }

                return borderPoints.at(y * bufferSize.x + x);
            };

            const auto setp = [&](const int x, const int y, const glm::ivec2 value) {
                if (not contains(x, y)) {
                    return;
                }

                borderPoints.at(y * bufferSize.x + x) = value;
            };

            for (int y = 0; y < bufferSize.y; ++y) {
                for (int x = 0; x < bufferSize.x; ++x) {
                    if (const auto center{I(x, y)};
                        I(x - 1, y) != center or I(x + 1, y) != center or I(x, y - 1) != center or I(x, y + 1) != center) {
                        setd(x, y, 0.0f);
                        setp(x, y, {x, y});
                        }
                    else {
                        setd(x, y, infinity);
                        setp(x, y, outOfBounds);
                    }
                }
            }

            const auto distance = [&](const int x, const int y) {
                const glm::ivec2 borderPoint{p(x, y)};
                return hypotf(static_cast<float>(x - borderPoint.x), static_cast<float>(y - borderPoint.y));
            };

            constexpr float distanceAdjacent{1.0f};
            constexpr float distanceDiagonal{1.41421356237f}; // sqrt(2.0)

            for (int y = 0; y < bufferSize.y; ++y) {
                for (int x = 0; x < bufferSize.x; ++x) {
                    if (d(x - 1, y - 1) + distanceDiagonal < d(x, y)) {
                        setp(x, y, p(x - 1, y - 1));
                        setd(x, y, distance(x, y));
                    }

                    if (d(x, y - 1) + distanceAdjacent < d(x, y)) {
                        setp(x, y, p(x, y - 1));
                        setd(x, y, distance(x, y));
                    }

                    if (d(x + 1, y - 1) + distanceDiagonal < d(x, y)) {
                        setp(x, y, p(x + 1, y - 1));
                        setd(x, y, distance(x, y));
                    }

                    if (d(x - 1, y) + distanceAdjacent < d(x, y)) {
                        setp(x, y, p(x - 1, y));
                        setd(x, y, distance(x, y));
                    }
                }
            }

            for (int y = bufferSize.y - 1; y >= 0; --y) {
                for (int x = bufferSize.x - 1; x >= 0; --x) {
                    if (d(x + 1, y) + distanceAdjacent < d(x, y)) {
                        setp(x, y, p(x + 1, y));
                        setd(x, y, distance(x, y));
                    }

                    if (d(x - 1, y + 1) + distanceDiagonal < d(x, y)) {
                        setp(x, y, p(x - 1, y + 1));
                        setd(x, y, distance(x, y));
                    }

                    if (d(x, y + 1) + distanceAdjacent < d(x, y)) {
                        setp(x, y, p(x, y + 1));
                        setd(x, y, distance(x, y));
                    }

                    if (d(x + 1, y + 1) + distanceDiagonal < d(x, y)) {
                        setp(x, y, p(x + 1, y + 1));
                        setd(x, y, distance(x, y));
                    }
                }
            }


            for (int y = 0; y < bufferSize.y; ++y) {
                for (int x = 0; x < bufferSize.x; ++x) {
                    if (I(x, y) == outside) {
                        setd(x, y, -d(x, y));
                    }
                }
            }

            return sdf;
        }

        /// Normalize an SDF and convert it to an 8-bit image.
        /// @param sdf A signed distance field.
        /// @param spread A factor that controls the range which is used to map the signed distance into the range of 0
        /// to 1 for storage in an 8-bit texture channel.
        /// @return A single channel image.
        std::vector<std::uint8_t> createImage(const std::vector<float>& sdf, const float spread = 8.0f) {
            std::vector<std::uint8_t> image(sdf.size());
            std::ranges::transform(sdf, image.begin(), [&](const float sdfValue) {
                const float normalizedValue{(sdfValue / spread + 1.0f) * 128.0f};
                return static_cast<std::uint8_t>(std::clamp(normalizedValue, 0.0f, 255.0f));
            });

            return image;
        }
    }

    std::vector<std::uint8_t> SignedDistanceField::createSDF(const std::uint8_t* bitmap, const glm::ivec2 bitmapSize,
                                        const glm::ivec2 paddedSize, const glm::ivec2 outputSize, const float spread) {
        const std::vector paddedBitmap{padImage(bitmap, bitmapSize, paddedSize)};
        const std::vector sdf{createFloatSDF(paddedBitmap.data(), paddedSize)};
        const std::vector sdfImage{createImage(sdf, spread)};
        std::vector<std::uint8_t> resizedSDFImage(outputSize.x * outputSize.y);
        stbir_resize_uint8_linear(sdfImage.data(), paddedSize.x, paddedSize.y, 0, resizedSDFImage.data(), outputSize.x,
                                  outputSize.y, 0, STBIR_1CHANNEL);

        return resizedSDFImage;
    }
} // namespace TileEngine::SignedDistanceField
