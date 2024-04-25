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
// Created by Anthony Dickson on 25/04/2024.
//

#include <numeric>

#include "TileTypes.h"

std::shared_ptr<TileTypes> TileTypes::create(const Size<int> sheetSize) {
    const std::vector vertexAttributeSizes{2, 2};
    const auto dataPointsPerVertex{std::reduce(vertexAttributeSizes.begin(), vertexAttributeSizes.end())};

    const auto width{1.0f / static_cast<float>(sheetSize.width)};
    const auto height{1.0f / static_cast<float>(sheetSize.height)};
    std::vector<float> vertexData(dataPointsPerVertex * 6);

    auto tileTypes{std::make_shared<TileTypes>()};
    tileTypes->tileTypes.reserve(sheetSize.width * sheetSize.height);

    for (int row = 0; row < sheetSize.height; ++row) {
        for (int col = 0; col < sheetSize.width; ++col) {
            const auto u{static_cast<float>(col) * width};
            const auto v{static_cast<float>(row) * height};

            vertexData = {
                // X, Y, U, V (2D coordinates, Texture coordinates).
                0.0f, 0.0f, u,         v + height,
                1.0f, 1.0f, u + width, v,
                0.0f, 1.0f, u, v,

                1.0f, 0.0f, u + width, v + height,
                1.0f, 1.0f, u + width, v,
                0.0f, 0.0f, u, v + height,
            };

            auto vao{std::make_unique<VertexArray>()};
            auto vbo{std::make_unique<VertexBuffer>()};
            vao->bind();
            vbo->loadData(vertexData, vertexAttributeSizes);
            tileTypes->tileTypes.emplace_back(std::move(vao), std::move(vbo));
        }
    }

    return tileTypes;
}

void TileTypes::render(const int tileID) const {
    const auto& [vao, vbo]{tileTypes.at(tileID)};
    vao->bind();
    vbo->drawArrays();
}
