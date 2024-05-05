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

#include <format>
#include <iostream>
#include <stdexcept>

#include <ft2build.h>
#include <stb_image.h>

#include <freetype/freetype.h>
#include <glm/ext/matrix_transform.hpp>
#include <stb_image_resize2.h>
#include <stb_image_write.h>

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/DeadReckoningAlgorithm.hpp>
#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {

    Font::Font(std::map<char, std::unique_ptr<Glyph>>& glyphs_, std::unique_ptr<VertexArray> vao_,
               std::unique_ptr<VertexBuffer> vbo_, std::unique_ptr<TextureArray> textureArray_) :
        glyphs(std::move(glyphs_)), vao(std::move(vao_)), vbo(std::move(vbo_)), textureArray(std::move(textureArray_)) {
    }

    std::unique_ptr<Font> Font::create(const std::string& fontPath) {
        // Code adapted from https://learnopengl.com/In-Practice/Text-Rendering and
        // https://github.com/johnWRS/LearnOpenGLTextRenderingImprovement.git
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
        }

        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
        }

        FT_Set_Pixel_Sizes(face, static_cast<FT_UInt>(fontSize.x), static_cast<FT_UInt>(fontSize.y));
        std::map<char, std::unique_ptr<Glyph>> glyphs;

        int unpackAlignment{};
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackAlignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        auto textureArray{TextureArray::create(charsToGenerate, fontSize)};

        for (unsigned char c = 0; c < charsToGenerate; c++) {
            // load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "ERROR::FREETYTPE: Failed to load Glyph: " << std::to_string(c) << std::endl;
                continue;
            }

            textureArray->bufferSubImage(c, {face->glyph->bitmap.width, face->glyph->bitmap.rows},
                                         face->glyph->bitmap.buffer);

            const glm::vec2 resolution(static_cast<float>(face->glyph->bitmap.width),
                                       static_cast<float>(face->glyph->bitmap.rows));
            const glm::vec2 bearing(static_cast<float>(face->glyph->bitmap_left),
                                    static_cast<float>(face->glyph->bitmap_top));
            // Divide advance by 64 to get the pixel spacing between characters since advance is in 1/64 units.
            const auto advance{static_cast<float>(face->glyph->advance.x >> 6)};

            auto character{std::make_unique<Glyph>(c, resolution, bearing, advance)};
            glyphs.emplace(c, std::move(character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment); // Restore unpack alignment.

        auto vao{std::make_unique<VertexArray>()};
        auto vbo{std::make_unique<VertexBuffer>()};
        vao->bind();
        vbo->loadData({0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f}, {2});

        return std::make_unique<Font>(glyphs, std::move(vao), std::move(vbo), std::move(textureArray));
    }

    std::unique_ptr<Font> Font::createSDF(const std::string& fontPath, const glm::ivec2 sdfFontSize,
                                          const glm::ivec2 textureSize, const float spread) {
        // Code adapted from https://learnopengl.com/In-Practice/Text-Rendering and
        // https://github.com/johnWRS/LearnOpenGLTextRenderingImprovement.git
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
        }

        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
        }

        FT_Set_Pixel_Sizes(face, sdfFontSize.x, sdfFontSize.y);
        std::map<char, std::unique_ptr<Glyph>> glyphs;

        int unpackAlignment{};
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpackAlignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

        auto textureArray{TextureArray::create(charsToGenerate, textureSize)};
        const DeadReckoningAlgorithm dra{};

        for (unsigned char c = 0; c < charsToGenerate; c++) {
            // load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "ERROR::FREETYTPE: Failed to load Glyph: " << std::to_string(c) << std::endl;
                continue;
            }

            const glm::vec2 resolution(static_cast<float>(face->glyph->bitmap.width),
                                       static_cast<float>(face->glyph->bitmap.rows));

            if (resolution.x != 0.0f and resolution.y != 0.0f) {
                const auto sdfStartTime{std::chrono::steady_clock::now()};
                auto sdf{dra.createSDF(face->glyph->bitmap.buffer, resolution, resolution)};

                const auto sdfImageStartTime{std::chrono::steady_clock::now()};
                auto sdfImage{dra.createImage(sdf, spread)};

                const auto resizeSDFImageTime{std::chrono::steady_clock::now()};
                std::uint8_t* resizedSDFImage{stbir_resize_uint8_linear(sdfImage.data(), resolution.x, resolution.y, 0,
                                                                        nullptr, textureSize.x, textureSize.y, 0,
                                                                        STBIR_1CHANNEL)};
                const auto endTime{std::chrono::steady_clock::now()};

                std::cout << std::format("Create SDF: {:.2f} ms - Create Image: {:.2f} ms - Resize Image: {:.2f} ms - "
                                         "Total: {:.2f} ms",
                                         (sdfImageStartTime - sdfStartTime).count() / 1e6f,
                                         (resizeSDFImageTime - sdfImageStartTime).count() / 1e6f,
                                         (endTime - resizeSDFImageTime).count() / 1e6f,
                                         (endTime - sdfStartTime).count() / 1e6f)
                          << '\n';
                textureArray->bufferSubImage(c, textureSize, resizedSDFImage);
                auto bitmap_path{std::format("resource/tmp/{:c}_bitmap.jpeg", c)};
                stbi_write_jpg(bitmap_path.c_str(), face->glyph->bitmap.width, face->glyph->bitmap.rows, 1,
                               face->glyph->bitmap.buffer, 90);
                auto sdf_path{std::format("resource/tmp/{:c}_sdf.png", c)};
                stbi_write_png(sdf_path.c_str(), textureSize.x, textureSize.y, 1, resizedSDFImage, sizeof(std::uint8_t) * textureSize.x);

                stbi_image_free(resizedSDFImage);
            }

            const glm::vec2 bearing(static_cast<float>(face->glyph->bitmap_left),
                                    static_cast<float>(face->glyph->bitmap_top));
            // Divide advance by 64 to get the pixel spacing between characters since advance is in 1/64 units.
            const auto advance{static_cast<float>(face->glyph->advance.x >> 6)};

            auto character{std::make_unique<Glyph>(c, resolution, bearing, advance)};
            glyphs.emplace(c, std::move(character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment); // Restore unpack alignment.

        auto vao{std::make_unique<VertexArray>()};
        auto vbo{std::make_unique<VertexBuffer>()};
        vao->bind();
        vbo->loadData({0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f}, {2});

        return std::make_unique<Font>(glyphs, std::move(vao), std::move(vbo), std::move(textureArray));
    }

    void Font::render(const std::string_view text, const glm::vec2 position, const float scale, const glm::vec3 colour,
                      const Camera& camera, const Font::Anchor anchor) const {
        shader.bind();
        shader.setUniform("text", 0);
        shader.setUniform("textColor", colour);
        shader.setUniform("projection", camera.getPerspectiveMatrix());

        textureArray->bind();
        vao->bind();
        vbo->bind();

        glm::vec2 drawPosition{position};

        const auto anchorOffset{calculateAnchorOffset(text, anchor)};
        int workingIndex{0};
        std::vector transforms(shader.maxInstances, glm::mat4());
        std::vector letterMap(shader.maxInstances, 0);

        const auto renderFn = [&] {
            glUniformMatrix4fv(shader.getUniformLocation("transforms"), workingIndex, GL_FALSE, &transforms[0][0][0]);
            glUniform1iv(shader.getUniformLocation("letterMap"), workingIndex, &letterMap[0]);
            glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, workingIndex);
        };

        for (const auto& character : text) {
            const auto& glyph{glyphs.at(character)};

            switch (character) {
            case ' ':
                drawPosition.x += glyph->advance * scale;
                continue;
            case '\n':
                drawPosition.y += fontSize.y * scale;
                drawPosition.x = position.x;
                continue;
            default:
                break;
            }

            const glm::vec2 screenCoordinates{drawPosition.x + (anchorOffset.x + glyph->bearing.x) * scale,
                                              drawPosition.y +
                                                  (anchorOffset.y + glyph->bearing.y - fontSize.y) * scale};
            const glm::vec2 size{fontSize * scale};

            glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), glm::vec3(screenCoordinates.x, screenCoordinates.y, -1.0f));
            transforms[workingIndex] = glm::scale(transform, glm::vec3(size.x, size.y, 0.0f));
            letterMap[workingIndex] = static_cast<int>(glyph->character);

            drawPosition.x += glyph->advance * scale;
            ++workingIndex;

            if (workingIndex == shader.maxInstances) {
                renderFn();
                workingIndex = 0;
            }
        }

        if (workingIndex > 0) {
            renderFn();
        }
    }

    glm::vec2 Font::calculateAnchorOffset(const std::string_view text, const Font::Anchor anchor) const {
        const auto textSize{calculateTextSize(text)};

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

    glm::vec2 Font::calculateTextSize(const std::string_view text) const {
        glm::vec2 textSize{};
        float lineWidth{};

        for (const auto& character : text) {
            const auto& glyph{glyphs.at(character)};

            if (character == '\n') {
                textSize.x = std::max(lineWidth, textSize.x);
                textSize.y += fontSize.y;
                lineWidth = 0.0f;
            }
            else {
                lineWidth += glyph->advance;
                textSize.y = std::max(fontSize.y, textSize.y);
            }
        }

        textSize.x = std::max(lineWidth, textSize.x);

        return textSize;
    }
} // namespace EconSimPlusPlus
