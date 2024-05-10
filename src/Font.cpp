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

#include <freetype/freetype.h>
#include <glm/ext/matrix_transform.hpp>

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/DeadReckoningAlgorithm.hpp>
#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {

    Font::Font(std::map<char, std::unique_ptr<Glyph>>& glyphs_, std::unique_ptr<VertexArray> vao_,
               std::unique_ptr<VertexBuffer> vbo_, std::unique_ptr<TextureArray> textureArray_,
               const glm::vec2 fontSize_) :
        glyphs(std::move(glyphs_)), vao(std::move(vao_)), vbo(std::move(vbo_)), textureArray(std::move(textureArray_)),
        fontSize(fontSize_) {
    }

    std::unique_ptr<Font> Font::create(const std::string& fontPath, const glm::ivec2 sdfFontSize,
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

        for (unsigned char c = 0; c < charsToGenerate; c++) {
            // load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "ERROR::FREETYTPE: Failed to load Glyph: " << std::to_string(c) << std::endl;
                continue;
            }

            const auto glyph{face->glyph};

            const glm::vec2 resolution(static_cast<float>(glyph->bitmap.width), static_cast<float>(glyph->bitmap.rows));

            if (resolution.x != 0.0f and resolution.y != 0.0f) {
                const auto sdf{DeadReckoningAlgorithm::createSDF(glyph->bitmap.buffer, resolution, sdfFontSize,
                                                                 textureSize, spread)};
                textureArray->bufferSubImage(c, textureSize, sdf.data());
            }

            const glm::vec2 paddedBearing{(static_cast<float>(sdfFontSize.x) - resolution.x) / 2,
                                          (static_cast<float>(sdfFontSize.y) - resolution.y) / 2};
            const glm::vec2 bearing(-paddedBearing.x + static_cast<float>(glyph->bitmap_left),
                                    paddedBearing.y + static_cast<float>(glyph->bitmap_top));
            // Divide advance by 64 to get the pixel spacing between characters since advance is in 1/64 units.
            const auto advance{static_cast<float>(glyph->advance.x >> 6)};

            // const glm::vec2 scale{1.0f, 1.0f};
            const glm::vec2 scale = static_cast<glm::vec2>(textureSize) / static_cast<glm::vec2>(sdfFontSize);
            auto character{std::make_unique<Glyph>(c, textureSize, bearing * scale, advance * scale.x)};
            glyphs.emplace(c, std::move(character));
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment); // Restore unpack alignment.

        auto vao{std::make_unique<VertexArray>()};
        auto vbo{std::make_unique<VertexBuffer>()};
        vao->bind();
        vbo->loadData({0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f}, {2});

        return std::make_unique<Font>(glyphs, std::move(vao), std::move(vbo), std::move(textureArray), textureSize);
    }

    void Font::render(const std::string_view text, const glm::vec3 position, const Camera& camera,
                      const RenderSettings& settings) const {
        shader.bind();
        shader.setUniform("text", 0);
        shader.setUniform("textColor", settings.color);
        shader.setUniform("projection", camera.getPerspectiveMatrix());
        shader.setUniform("sdfThreshold", settings.sdfThreshold);
        shader.setUniform("edgeSmoothness", settings.edgeSmoothness);
        shader.setUniform("outlineSize", settings.outlineSize);
        shader.setUniform("outlineColor", settings.outlineColor);

        textureArray->bind();
        vao->bind();
        vbo->bind();

        glm::vec3 drawPosition{position};

        const float scale{settings.size / fontSize.y};
        const auto anchorOffset{calculateAnchorOffset(text, settings.anchor) * scale};
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
                drawPosition.y -= fontSize.y * scale;
                drawPosition.x = position.x;
                continue;
            default:
                break;
            }

            const glm::vec2 screenCoordinates{drawPosition.x + anchorOffset.x + glyph->bearing.x * scale,
                                              drawPosition.y + anchorOffset.y +
                                                  (glyph->bearing.y - glyph->size.y) * scale};

            glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), glm::vec3(screenCoordinates.x, screenCoordinates.y, -drawPosition.z));
            transforms[workingIndex] = glm::scale(transform, glm::vec3(fontSize * scale, 0.0f));
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

        // -fontSize.y puts the text origin at the top left corner of the first character.
        switch (anchor) {
        case Anchor::bottomLeft:
            return glm::vec2{0.0f, textSize.y - fontSize.y};
        case Anchor::bottomRight:
            return glm::vec2{-textSize.x, textSize.y - fontSize.y};
        case Anchor::topLeft:
            return glm::vec2{0.0f, -fontSize.y};
        case Anchor::topRight:
            return glm::vec2{-textSize.x, -fontSize.y};
        case Anchor::center:
            return {-textSize.x / 2.0f, textSize.y / 2.0f - fontSize.y};
        default:
            return glm::vec2{0.0f};
        }
    }

    glm::vec2 Font::calculateTextSize(const std::string_view text) const {
        glm::vec2 textSize{0.0f, fontSize.y};
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
            }
        }

        textSize.x = std::max(lineWidth, textSize.x);
        textSize.y = std::max(fontSize.y, textSize.y);

        return textSize;
    }
} // namespace EconSimPlusPlus
