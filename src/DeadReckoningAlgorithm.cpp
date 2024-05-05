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
// Created by Anthony Dickson on 04/05/2024.
//

#include <algorithm>

#include <EconSimPlusPlus/DeadReckoningAlgorithm.hpp>
#include <glm/detail/func_vector_relational.inl>

namespace EconSimPlusPlus {
    DeadReckoningAlgorithm::DeadReckoningAlgorithm(const float distanceAdjacent_, const float distanceDiagonal_) :
        distanceAdjacent(distanceAdjacent_), distanceDiagonal(distanceDiagonal_) {
    }

    std::vector<float> DeadReckoningAlgorithm::createSDF(const std::uint8_t* buffer, const glm::ivec2 bufferSize,
                                                         const glm::ivec2 outputSize) const {
        assert(outputSize.x >= bufferSize.x and outputSize.y >= bufferSize.y &&
               "Output size must be greater than or equal to the buffer size.");
        std::vector distanceImage(outputSize.x * outputSize.y, std::numeric_limits<float>::infinity());
        std::vector<glm::ivec2> borderPoints(outputSize.x * outputSize.y, {-1, -1});
        // const glm::ivec2 padding{(outputSize - bufferSize) / 2};
        //
        // const auto I = [&](const int x, const int y) {
        //     const auto bufferX{x - padding.x};
        //     const auto bufferY{y - padding.y};
        //
        //     if (bufferX < 0 or bufferX >= bufferSize.x or bufferY < 0 or bufferY >= bufferSize.y) {
        //         return false;
        //     }
        //
        //     return buffer[bufferY * bufferSize.x + bufferX] > 0;
        // };
        const auto I = [&](const int x, const int y) {return buffer[y * bufferSize.x + x] > 0;};
        const auto d = [&](const int x, const int y) { return distanceImage[y * outputSize.x + x]; };
        const auto setd = [&](const int x, const int y, const float value) {
            return distanceImage[y * outputSize.x + x] = value;
        };
        const auto p = [&](const int x, const int y) { return borderPoints[y * outputSize.x + x]; };
        const auto setp = [&](const int x, const int y, const glm::ivec2 value) { borderPoints[y * outputSize.x + x] = value; };

        const auto d1{distanceAdjacent};
        const auto d2{distanceDiagonal};

        // Inintialise immediate interior and exterior elements.
        for (int y = 1; y < outputSize.y; ++y) {
            for (int x = 1; x < outputSize.x; ++x) {
                if (I(x - 1, y) != I(x, y) or I(x + 1, y) != I(x, y) or I(x, y - 1) != I(x, y) or
                    I(x, y + 1) == I(x, y)) {
                    setd(x, y, 0.0f);
                    setp(x, y, {x, y});
                }
            }
        }

        // First Pass
        for (int y = 1; y < outputSize.y; ++y) {
            for (int x = 1; x < outputSize.x; ++x) {
                if (d(x - 1, y - 1) + d2 < d(x, y)) {
                    setp(x, y, {x - 1, y - 1});
                    setd(x, y, hypotf(x - p(x, y).x, y - p(x, y).y));
                }
                else if (d(x, y - 1) + d1 < d(x, y)) {
                    setp(x, y, {x, y - 1});
                    setd(x, y, hypotf(x - p(x, y).x, y - p(x, y).y));
                }
                else if (d(x + 1, y - 1) + d2 < d(x, y)) {
                    setp(x, y, {x + 1, y - 1});
                    setd(x, y, hypotf(x - p(x, y).x, y - p(x, y).y));
                }
                else if (d(x - 1, y) + d1 < d(x, y)) {
                    setp(x, y, {x - 1, y});
                    setd(x, y, hypotf(x - p(x, y).x, y - p(x, y).y));
                }
            }
        }

        // Final Pass
        for (int y = outputSize.y - 1; y > -1; --y) {
            for (int x = outputSize.x - 1; x > -1; --x) {
                if (d(x + 1, y) + d1 < d(x, y)) {
                    setp(x, y, {x + 1, y});
                    setd(x, y, hypotf(x - p(x, y).x, y - p(x, y).y));
                }
                else if (d(x - 1, y + 1) + d2 < d(x, y)) {
                    setp(x, y, {x - 1, y + 1});
                    setd(x, y, hypotf(x - p(x, y).x, y - p(x, y).y));
                }
                else if (d(x, y + 1) + d1 < d(x, y)) {
                    setp(x, y, {x, y + 1});
                    setd(x, y, hypotf(x - p(x, y).x, y - p(x, y).y));
                }
                else if (d(x + 1, y + 1) + d2 < d(x, y)) {
                    setp(x, y, {x + 1, y + 1});
                    setd(x, y, hypotf(x - p(x, y).x, y - p(x, y).y));
                }
            }
        }

        // Indicate inside and outside
        for (int y = 0; y < outputSize.y; ++y) {
            for (int x = 0; x < outputSize.x; ++x) {
                if (I(x, y) == 0) {
                    setd(x, y, -d(x, y));
                }
            }
        }

        return distanceImage;
    }

    std::vector<std::uint8_t> DeadReckoningAlgorithm::createImage(const std::vector<float>& sdf, const float spread) {
        std::vector<std::uint8_t> image(sdf.size());
        std::ranges::transform(sdf, image.begin(), [&](const float sdfValue) {
            const float normalizedValue{(sdfValue / spread + 1.0f) * 128.0f};
            return static_cast<int>(std::clamp(normalizedValue, 0.0f, 255.0f));
        });

        return image;
    }
} // namespace EconSimPlusPlus
