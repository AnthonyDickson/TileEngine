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

namespace EconSimPlusPlus {
    DeadReckoningAlgorithm::DeadReckoningAlgorithm(const float distanceAdjacent_, const float distanceDiagonal_) :
        distanceAdjacent(distanceAdjacent_), distanceDiagonal(distanceDiagonal_) {
    }

    std::vector<float> DeadReckoningAlgorithm::sdf(const std::uint8_t* buffer, const glm::ivec2 bufferSize) const {
        assert(bufferSize.x >= bufferSize.x and bufferSize.y >= bufferSize.y &&
               "Output size must be greater than or equal to the buffer size in both dimensions.");
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
            const auto borderPoint{p(x, y)};
            return hypotf(x - borderPoint.x, y - borderPoint.y);
        };

        const auto d1{distanceAdjacent};
        const auto d2{distanceDiagonal};

        for (int y = 0; y < bufferSize.y; ++y) {
            for (int x = 0; x < bufferSize.x; ++x) {
                if (d(x - 1, y - 1) + d2 < d(x, y)) {
                    setp(x, y, p(x - 1, y - 1));
                    setd(x, y, distance(x, y));
                }

                if (d(x, y - 1) + d1 < d(x, y)) {
                    setp(x, y, p(x, y - 1));
                    setd(x, y, distance(x, y));
                }

                if (d(x + 1, y - 1) + d2 < d(x, y)) {
                    setp(x, y, p(x + 1, y - 1));
                    setd(x, y, distance(x, y));
                }

                if (d(x - 1, y) + d1 < d(x, y)) {
                    setp(x, y, p(x - 1, y));
                    setd(x, y, distance(x, y));
                }
            }
        }

        for (int y = bufferSize.y - 1; y >= 0; --y) {
            for (int x = bufferSize.x - 1; x >= 0; --x) {
                if (d(x + 1, y) + d1 < d(x, y)) {
                    setp(x, y, p(x + 1, y));
                    setd(x, y, distance(x, y));
                }

                if (d(x - 1, y + 1) + d2 < d(x, y)) {
                    setp(x, y, p(x - 1, y + 1));
                    setd(x, y, distance(x, y));
                }

                if (d(x, y + 1) + d1 < d(x, y)) {
                    setp(x, y, p(x, y + 1));
                    setd(x, y, distance(x, y));
                }

                if (d(x + 1, y + 1) + d2 < d(x, y)) {
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

    std::vector<std::uint8_t> DeadReckoningAlgorithm::createImage(const std::vector<float>& sdf, const float spread) {
        std::vector<std::uint8_t> image(sdf.size());
        std::ranges::transform(sdf, image.begin(), [&](const float sdfValue) {
            const float normalizedValue{(sdfValue / spread + 1.0f) * 128.0f};
            return static_cast<std::uint8_t>(std::clamp(normalizedValue, 0.0f, 255.0f));
        });

        return image;
    }
} // namespace EconSimPlusPlus
