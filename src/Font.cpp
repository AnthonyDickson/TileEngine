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
// Created by Anthony Dickson on 26/04/2024.
//

#include <iostream>
#include <stdexcept>

// ReSharper disable once CppUnusedIncludeDirective
#include <ft2build.h>

#include <freetype/freetype.h>

#include <EconSimPlusPlus/Font.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "EconSimPlusPlus/VertexArray.hpp"
#include "EconSimPlusPlus/VertexBuffer.hpp"

namespace EconSimPlusPlus {
    Font::Font(std::map<char, std::unique_ptr<Glyph>>& glyphs_, std::unique_ptr<VertexArray> vao_,
               std::unique_ptr<VertexBuffer> vbo_) :
        glyphs(std::move(glyphs_)), vao(std::move(vao_)), vbo(std::move(vbo_)) {
    }

    std::unique_ptr<Font> Font::create(const std::string& fontPath) {
        // Code adapted from https://learnopengl.com/In-Practice/Text-Rendering
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
        }

        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
        }

        FT_Set_Pixel_Sizes(face, 0, 48);
        std::map<char, std::unique_ptr<Glyph>> glyphs;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        for (unsigned char c = 0; c < 128; c++) {
            // load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "ERROR::FREETYTPE: Failed to load Glyph: " << std::to_string(c) << std::endl;
                continue;
            }

            glm::ivec2 size(static_cast<int>(face->glyph->bitmap.width), static_cast<int>(face->glyph->bitmap.rows));

            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size.x, size.y, 0, GL_RED, GL_UNSIGNED_BYTE,
                         face->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            auto character{std::make_unique<Glyph>(
                texture, size, glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x)};
            glyphs.emplace(c, std::move(character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        auto vao{std::make_unique<VertexArray>()};
        auto vbo{std::make_unique<VertexBuffer>()};
        vao->bind();
        vbo->bind();
        // 6 * 4 for six vertices with 2D position and texture coordinates.
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        return std::make_unique<Font>(glyphs, std::move(vao), std::move(vbo));
    }

    void Font::render(const std::string_view text, const glm::vec2 position, const float scale, const glm::vec3 colour,
                      const Camera& camera, const Font::Anchor anchor) const {
        const glm::mat4 projection = camera.getPerspectiveMatrix();
        shader.bind();
        shader.setUniform("text", 0);
        shader.setUniform("textColor", colour);
        shader.setUniform("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        vao->bind();

        glm::vec2 drawPosition{position};

        const auto anchorOffset{calculateAnchorOffset(text, anchor, scale)};

        for (const auto& character : text) {
            const auto& glyph{glyphs.at(character)};

            const glm::vec2 screenCoordinates{drawPosition.x + static_cast<float>(glyph->bearing.x) * scale,
                                              drawPosition.y -
                                                  static_cast<float>(glyph->size.y - glyph->bearing.y) * scale};

            const glm::vec2 size{static_cast<glm::vec2>(glyph->size) * scale};

            const float x = anchorOffset.x + screenCoordinates.x;
            const float y = anchorOffset.y + screenCoordinates.y;

            const float vertices[6][4]{
                {x, y + size.y, 0.0f, 0.0f}, {x, y, 0.0f, 1.0f},          {x + size.x, y, 1.0f, 1.0f},
                {x, y + size.y, 0.0f, 0.0f}, {x + size.x, y, 1.0f, 1.0f}, {x + size.x, y + size.y, 1.0f, 0.0f},
            };

            glyph->bind();
            vbo->bind();
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            drawPosition.x += static_cast<float>(glyph->advance >> 6) * scale;
        }
    }

    glm::vec2 Font::calculateAnchorOffset(const std::string_view text, const Font::Anchor anchor,
                                          const float scale) const {
        glm::vec2 textSize{};

        for (const auto& character : text) {
            const auto& glyph{glyphs.at(character)};

            textSize.x += static_cast<float>(glyph->advance >> 6) * scale;
            textSize.y = static_cast<float>(glyph->size.y) * scale;
        }

        switch (anchor) {
        case Anchor::bottomLeft:
            return glm::vec2{0.0f};
        case Anchor::bottomRight:
            return glm::vec2{-textSize.x, 0.0f};
        case Anchor::topLeft:
            return glm::vec2{0.0f, -textSize.y};
        case Anchor::topRight:
            return -textSize;
        case Anchor::center:
            return -textSize / 2.0f;
        default:
            return glm::vec2{0.0f};
        }
    }
} // namespace EconSimPlusPlus
