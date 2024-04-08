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
// Created by Anthony on 31/10/2023.
//
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Tile.h"
#include "TileGrid.h"
#include "TileRegistry.h"
#include "Window.h"
#include "VertexArray.h"
#include "VertexBuffer.h"


int main() {
    // TODO: Moving 'the camera' should simply change the row/col offset.
    std::cout << "Hello, World!" << std::endl;

    constexpr int windowWidth{1440};
    constexpr int windowHeight{1080};

    Window window{windowWidth, windowHeight, "EconSimPlusPlus"};

    Camera camera{{windowWidth, windowHeight},
                  {0.0f,        0.0f, 3.0f}};

    // Create the vertex array object.
    VertexArray vao{};
    vao.bind();

    int tilesPerHeight{32};
    int tileSize{windowHeight / tilesPerHeight};
    int tilesPerWidth{windowWidth / tileSize};
    std::vector<float> vertexData{
            // X, Y, U, V (2D coordinates, Texture coordinates).
            // [0]    -> [1,4]
            //         /
            //       /
            // [2, 5] -> [3]
            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
    };
    VertexBuffer vbo{vertexData, 4, std::vector<int>{2, 2}};

    Shader shader{"resource/shader/tile.vert", "resource/shader/tile.frag"};

    auto container{std::make_shared<const Texture>("resource/container2.png", GL_TEXTURE0)};
    auto awesomeFace{std::make_shared<const Texture>("resource/awesomeface.png", GL_TEXTURE0)};

    TileRegistry tileRegistry{};
    tileRegistry.emplace(container);
    tileRegistry.emplace(awesomeFace);
    TileGrid tileGrid{64, 64};
    tileGrid[0, 0] = 1;
    tileGrid[31, 31] = 1;
    tileGrid[63, 63] = 1;

    auto update = [&](float deltaTime) {
        if (window.getKeyState(GLFW_KEY_ESCAPE)) {
            window.close();
            return;
        }

        if (window.hasWindowSizeChanged()) {
            camera.onWindowResize({static_cast<float>(window.getWidth()),
                                   static_cast<float>(window.getHeight())});
            tileSize = window.getHeight() / tilesPerHeight;
            tilesPerWidth = window.getWidth() / tileSize;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        const glm::mat4 projectionViewMatrix = camera.getPerspectiveMatrix() * camera.getViewMatrix();

        shader.bind();
        shader.setUniform("projectionViewMatrix", projectionViewMatrix);
        vao.bind();

        for (int row = 0; row < tilesPerHeight; ++row) {
            for (int col = 0; col < tilesPerWidth; ++col) {
                glm::mat4 model{1.0f};
                model = glm::translate(
                        model,
                        glm::vec3{static_cast<float>(col * tileSize), static_cast<float>(row * tileSize), 0.0}
                );
                model = glm::scale(model, glm::vec3{static_cast<float>(tileSize), static_cast<float>(tileSize), 0.0});

                shader.setUniform("model", model);
                const auto tileID = tileGrid[row, col];
                tileRegistry[tileID].bind();
                vbo.drawArrays();
            }
        }
    };

    try {
        window.runMainLoop(update);
    } catch (const std::exception &exception) {
        std::cout << "Program exited with unhandled exception: " << exception.what() << std::endl;
    }

    return 0;
}
