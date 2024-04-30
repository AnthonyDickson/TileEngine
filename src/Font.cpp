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


#include <ft2build.h>

#include <freetype/freetype.h>

#include <glm/ext/matrix_transform.hpp>

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

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

        int unpackAlignment{};
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackAlignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        for (unsigned char c = 0; c < 128; c++) {
            // load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "ERROR::FREETYTPE: Failed to load Glyph: " << std::to_string(c) << std::endl;
                continue;
            }

            glm::vec2 size(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows));

            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y), 0,
                         GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            const glm::vec2 bearing(static_cast<float>(face->glyph->bitmap_left),
                                    static_cast<float>(face->glyph->bitmap_top));
            // Divide advance by 64 to get the pixel spacing between characters since advance is in 1/64 units.
            const auto advance{static_cast<float>(face->glyph->advance.x >> 6)};

            auto character{std::make_unique<Glyph>(texture, size, bearing, advance)};
            glyphs.emplace(c, std::move(character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment); // Restore unpack alignment.

        auto vao{std::make_unique<VertexArray>()};
        auto vbo{std::make_unique<VertexBuffer>()};
        vao->bind();
        vbo->loadData({0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f}, {2});

        return std::make_unique<Font>(glyphs, std::move(vao), std::move(vbo));
    }

    void Font::render(const std::string_view text, const glm::vec2 position, const float scale, const glm::vec3 colour,
                      const Camera& camera, const Font::Anchor anchor) const {
        shader.bind();
        shader.setUniform("text", 0);
        shader.setUniform("textColor", colour);
        shader.setUniform("projection", camera.getPerspectiveMatrix());

        glActiveTexture(GL_TEXTURE0);
        vao->bind();

        glm::vec2 drawPosition{position};

        const auto anchorOffset{calculateAnchorOffset(text, anchor)};

        for (const auto& character : text) {
            const auto& glyph{glyphs.at(character)};

            switch (character) {
            case ' ':
                drawPosition.x += glyph->advance;
                continue;
            case '\n':
                drawPosition.y += glyph->size.y;
                drawPosition.x = position.x;
                continue;
            default:
                break;
            }

            const glm::vec2 screenCoordinates{(anchorOffset.x + drawPosition.x + glyph->bearing.x) * scale,
                                              (anchorOffset.y + drawPosition.y + glyph->bearing.y - glyph->size.y) *
                                                  scale};
            const glm::vec2 size{glyph->size * scale};

            glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), glm::vec3(screenCoordinates.x, screenCoordinates.y, -1.0f));
            transform = glm::scale(transform, glm::vec3(size.x, size.y, 0.0f));
            shader.setUniform("transform", transform);

            glyph->bind();
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            drawPosition.x += glyph->advance;
        }
    }

    glm::vec2 Font::calculateAnchorOffset(const std::string_view text, const Font::Anchor anchor) const {
        const auto textSize{calculateTextSize(text)};

        switch (anchor) {
        case Anchor::bottomLeft:
            return glm::vec2{0.0f, -textSize.y};
        case Anchor::bottomRight:
            return -textSize;
        case Anchor::topLeft:
            return glm::vec2{0.0f};
        case Anchor::topRight:
            return glm::vec2{-textSize.x, 0.0f};
        case Anchor::center:
            return -textSize / 2.0f;
        default:
            return glm::vec2{0.0f};
        }
    }

    glm::vec2 Font::calculateTextSize(const std::string_view text) const {
        glm::vec2 textSize{};
        float lineWidth{};

        for (const auto& character : text) {
            const auto& glyph{glyphs.at(character)};

            if (character == '\n') {
                textSize.x = std::max(lineWidth, textSize.x);
                textSize.y += glyph->size.y;
                lineWidth = 0.0f;
            }
            else {
                lineWidth += glyph->advance;
                textSize.y = std::max(glyph->size.y, textSize.y);
            }
        }

        textSize.x = std::max(lineWidth, textSize.x);

        return textSize;
    }
} // namespace EconSimPlusPlus
