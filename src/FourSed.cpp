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

#include <algorithm>
#include <unordered_map>

#include <EconSimPlusPlus/FourSed.hpp>

namespace EconSimPlusPlus {
    std::vector<float> FourSED::edt(const std::uint8_t* binaryImage, const glm::ivec2 inputSize) {
        std::vector<glm::ivec2> df(inputSize.x * inputSize.y);

        constexpr glm::ivec2 inside{0};
        constexpr glm::ivec2 outside{std::numeric_limits<int>::max() - 1};

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
                auto a = df.at(index(col, row));
                auto b = df.at(index(col, row - 1)) + south;
                auto minVec = min(a, b);
                df.at(index(col, row)) = minVec;
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

        std::vector<float> floatSDF(inputSize.x * inputSize.y);

        std::ranges::transform(df.begin(), df.end(), floatSDF.begin(), [](const auto& distanceVector) {
            return hypotf(static_cast<float>(distanceVector.x), static_cast<float>(distanceVector.y));
        });

        return floatSDF;
    }

    std::vector<std::uint8_t> FourSED::createImage(const std::vector<float>& distanceField) {
        std::vector<std::uint8_t> image(distanceField.size());
        const auto max{std::ranges::max(distanceField)};

        std::ranges::transform(distanceField, std::back_inserter(image), [&](const float sdfValue) {
            return static_cast<int>(sdfValue / max * 255.0f);
        });

        return image;
    }
} // namespace EconSimPlusPlus
