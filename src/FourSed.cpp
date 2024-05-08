//  EconSimPlusPlus
//  A program that simulates the historical development of capitalist economies.
//  Copyright (C) 2024.   Anthony Dickson anthony.dickson9656@gmail.com
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by Anthony Dickson on 07/05/2024.
//

#include <EconSimPlusPlus/Camera.hpp>
#include <algorithm>
#include <stb_image.h>
#include <stb_image_resize2.h>
#include <unordered_map>

#include <EconSimPlusPlus/FourSed.hpp>

namespace EconSimPlusPlus {
    template <typename PixelType>
    std::vector<PixelType> FourSED::padImage(const PixelType* binaryImage, const glm::ivec2 inputSize,
                                             const glm::ivec2 outputSize) {
        auto padding{(outputSize - inputSize) / 2};
        // Make sure padding is at least zero to avoid negative indices. This may happen when the glyph is larger than
        // the requested size.
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
    std::vector<float> FourSED::edt(const std::uint8_t* binaryImage, const glm::ivec2 inputSize, const bool flipBits) {
        std::vector<glm::ivec2> df(inputSize.x * inputSize.y);

        const glm::ivec2 inside = flipBits ? glm::ivec2{std::numeric_limits<int>::max() - 1} : glm::ivec2{0};
        const glm::ivec2 outside = flipBits ? glm::ivec2{0} : glm::ivec2{std::numeric_limits<int>::max() - 1};

        for (int i = 0; i < inputSize.x * inputSize.y; ++i) {
            df.at(i) = binaryImage[i] > 128 ? inside : outside;
        }

        constexpr glm::ivec2 east{1, 0};
        constexpr glm::ivec2 south{0, 1};

        const auto index = [&](const int col, const int row) { return row * inputSize.x + col; };

        const auto squareSum = [](const glm::ivec2& vector) -> float { return pow(vector.x, 2) + pow(vector.y, 2); };

        const auto min = [&](const glm::ivec2 a, const glm::ivec2 b) {
            // Define the minimum of two vectors as the vector with the smallest magnitude.
            // Here we compare the squared distance instead of the Euclidean distance as a small optimisation.
            if (squareSum(a) < squareSum(b)) {
                return a;
            }

            return b;
        };

        for (int row = 1; row < inputSize.y; ++row) {
            for (int col = 0; col < inputSize.x; ++col) {
                df.at(index(col, row)) = min(df.at(index(col, row)), df.at(index(col, row - 1)) + south);
            }

            for (int col = 1; col < inputSize.x; ++col) {
                df.at(index(col, row)) = min(df.at(index(col, row)), df.at(index(col - 1, row)) + east);
            }

            for (int col = inputSize.x - 2; col >= 0; --col) {
                df.at(index(col, row)) = min(df.at(index(col, row)), df.at(index(col + 1, row)) + east);
            }
        }

        for (int row = inputSize.y - 2; row >= 0; --row) {
            for (int col = 0; col < inputSize.x; ++col) {
                df.at(index(col, row)) = min(df.at(index(col, row)), df.at(index(col, row + 1)) + south);
            }

            for (int col = 1; col < inputSize.x; ++col) {
                df.at(index(col, row)) = min(df.at(index(col, row)), df.at(index(col - 1, row)) + east);
            }

            for (int col = inputSize.x - 2; col >= 0; --col) {
                df.at(index(col, row)) = min(df.at(index(col, row)), df.at(index(col + 1, row)) + east);
            }
        }

        std::vector<float> floatDF(inputSize.x * inputSize.y);

        std::ranges::transform(df, floatDF.begin(), [](const auto& distanceVector) {
            return hypotf(static_cast<float>(distanceVector.x), static_cast<float>(distanceVector.y));
        });

        return floatDF;
    }

    std::vector<std::uint8_t> FourSED::combineEDT(const std::vector<float>& insideDistanceField,
                                                   const std::vector<float>& outsideDistanceField, const float spread) {
        assert(insideDistanceField.size() == outsideDistanceField.size() &&
               "Distance fields must be of the same size.");
        std::vector<std::uint8_t> image(insideDistanceField.size());

        for (std::size_t i = 0; i < insideDistanceField.size(); ++i) {
            const auto inside{insideDistanceField.at(i)};
            const auto outside{-outsideDistanceField.at(i)};
            const auto normalizedValue{std::clamp(((inside + outside) / spread + 1.0f) * 128.0f, 0.0f, 255.0f)};
            image.at(i) = static_cast<std::uint8_t>(normalizedValue);
        }

        return image;
    }

    std::vector<std::uint8_t> FourSED::createSDF(const std::uint8_t* bitmap, const glm::ivec2 bitmapSize,
                                                 const glm::ivec2 paddedSize, const glm::ivec2 outputSize,
                                                 const float spread) {
        const auto paddedBitmap{FourSED::padImage(bitmap, bitmapSize, paddedSize)};
        const auto outsideDF{FourSED::edt(paddedBitmap.data(), paddedSize)};

        // We run the EDT on the bitmap for calculating the internal distances without padding.
        // This saves time since we skip calculating the distance for pixels outside the objects, which will be zero
        // anyway.
        const auto minimalPadSize = bitmapSize + 2;
        const auto minimallyPaddedBitmap{FourSED::padImage(bitmap, bitmapSize, minimalPadSize)};
        const auto minimallyPaddedInsideDF{FourSED::edt(minimallyPaddedBitmap.data(), minimalPadSize, true)};
        const auto insideDF{FourSED::padImage(minimallyPaddedInsideDF.data(), minimalPadSize, paddedSize)};

        const auto sdfImage{FourSED::combineEDT(insideDF, outsideDF, spread)};
        const auto resizedSDFImage{stbir_resize_uint8_linear(sdfImage.data(), paddedSize.x, paddedSize.y, 0, nullptr,
                                                             outputSize.x, outputSize.y, 0, STBIR_1CHANNEL)};

        std::vector<std::uint8_t> sdf{resizedSDFImage, resizedSDFImage + outputSize.x * outputSize.y};
        stbi_image_free(resizedSDFImage);

        return sdf;
    }
} // namespace EconSimPlusPlus
