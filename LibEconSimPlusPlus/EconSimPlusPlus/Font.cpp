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
#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/SignedDistanceField.hpp>

namespace EconSimPlusPlus {
    namespace {
        /// The range of ASCII characters to generate glyphs for.
        constexpr int charsToGenerate{128};
    } // namespace

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

        std::unique_ptr textureArray{TextureArray::create(charsToGenerate, textureSize)};
        glm::vec2 verticalExtents{};

        for (unsigned char c = 0; c < charsToGenerate; c++) {
            // load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "ERROR::FREETYTPE: Failed to load Glyph: " << std::to_string(c) << std::endl;
                continue;
            }

            const FT_GlyphSlotRec_* glyph{face->glyph};

            const glm::vec2 resolution(static_cast<float>(glyph->bitmap.width), static_cast<float>(glyph->bitmap.rows));

            if (resolution.x != 0.0f and resolution.y != 0.0f) {
                const std::vector sdf{
                    SignedDistanceField::createSDF(glyph->bitmap.buffer, resolution, sdfFontSize, textureSize, spread)};
                textureArray->bufferSubImage(c, textureSize, sdf.data());
            }

            const glm::vec2 paddedBearing{(static_cast<float>(sdfFontSize.x) - resolution.x) / 2,
                                          (static_cast<float>(sdfFontSize.y) - resolution.y) / 2};
            const glm::vec2 bearing(-paddedBearing.x + static_cast<float>(glyph->bitmap_left),
                                    paddedBearing.y + static_cast<float>(glyph->bitmap_top));
            // Divide advance by 64 to get the pixel spacing between characters since advance is in 1/64 units.
            const auto advance{static_cast<float>(glyph->advance.x >> 6)};

            const glm::vec2 scale = static_cast<glm::vec2>(textureSize) / static_cast<glm::vec2>(sdfFontSize);
            auto character{std::make_unique<Glyph>(c, textureSize, bearing * scale, advance * scale.x)};
            glyphs.emplace(c, std::move(character));

            const float distanceAboveBaseline = bearing.y;
            const float distanceBelowBaseline = bearing.y - resolution.y;
            verticalExtents.x = std::min(verticalExtents.x, distanceBelowBaseline * scale.y);
            verticalExtents.y = std::max(verticalExtents.y, distanceAboveBaseline * scale.y);
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment); // Restore unpack alignment.

        return std::make_unique<Font>(glyphs, std::move(textureArray), textureSize, verticalExtents);
    }

    Font::Font(std::map<char, std::unique_ptr<Glyph>>& glyphs, std::unique_ptr<TextureArray> textureArray,
               const glm::vec2 fontSize, const glm::vec2 verticalExtents) :
        m_glyphs(std::move(glyphs)), m_fontSize(fontSize), m_verticalExtents(verticalExtents),
        m_textureArray(std::move(textureArray)) {
    }

    float Font::calculateScaleFactor(const Style& style) const {
        return style.size / (abs(m_verticalExtents.x) + m_verticalExtents.y);
    }

    glm::vec2 Font::calculateTextSize(const std::string_view text) const {
        glm::vec2 textSize{0.0f, m_verticalExtents.y};
        float lineWidth{};

        for (const auto& character : text) {
            const std::unique_ptr<Glyph>& glyph{m_glyphs.at(character)};

            if (character == '\n') {
                textSize.x = std::max(lineWidth, textSize.x);
                textSize.y += m_verticalExtents.y;
                lineWidth = 0.0f;
            }
            else {
                lineWidth += glyph->advance;
            }
        }

        textSize.x = std::max(lineWidth, textSize.x);
        textSize.y += abs(m_verticalExtents.x);

        return textSize;
    }

    void Font::render(const std::string_view text, const glm::vec3 position, const Camera& camera,
                      const Style& style) const {
        m_shader.bind();
        m_shader.setUniform("text", 0);
        m_shader.setUniform("textColor", style.color);
        m_shader.setUniform("projectionViewMatrix", camera.perspectiveMatrix() * camera.viewMatrix());
        m_shader.setUniform("sdfThreshold", style.sdfThreshold);
        m_shader.setUniform("edgeSmoothness", style.edgeSmoothness);
        m_shader.setUniform("outlineSize", style.outlineSize);
        m_shader.setUniform("outlineColor", style.outlineColor);

        m_textureArray->bind();

        glm::vec3 drawPosition{position + glm::vec3{style.padding.x / 2.0f, -style.padding.y / 2.0f, 0.0f}};

        const float scale{calculateScaleFactor(style)};
        const glm::vec2 textSize{calculateTextSize(text) + style.padding};
        // The `m_fontSize.y` puts the text origin at the top left corner of the first character.
        const glm::vec2 anchorOffset{calculateAnchorOffset(textSize, style.anchor, m_fontSize.y) * scale};
        int workingIndex{0};
        std::vector transforms(m_shader.maxInstances(), glm::mat4());
        std::vector letterMap(m_shader.maxInstances(), 0);

        const auto renderFn = [&] {
            glUniformMatrix4fv(m_shader.uniformLocation("transforms"), workingIndex, GL_FALSE, &transforms[0][0][0]);
            glUniform1iv(m_shader.uniformLocation("letterMap"), workingIndex, &letterMap[0]);
            m_quad.render(workingIndex, GL_TRIANGLE_STRIP);
        };

        for (const auto& character : text) {
            const std::unique_ptr<Glyph>& glyph{m_glyphs.at(character)};

            switch (character) {
            case ' ':
                drawPosition.x += glyph->advance * scale;
                continue;
            case '\n':
                drawPosition.y -= m_verticalExtents.y * scale;
                drawPosition.x = position.x + style.padding.x / 2.0f;
                continue;
            default:
                break;
            }

            const glm::vec2 screenCoordinates{drawPosition.x + anchorOffset.x + glyph->bearing.x * scale,
                                              drawPosition.y + anchorOffset.y +
                                                  (glyph->bearing.y - glyph->size.y) * scale};

            glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), glm::vec3(screenCoordinates.x, screenCoordinates.y, drawPosition.z));
            transforms[workingIndex] = glm::scale(transform, glm::vec3(m_fontSize * scale, 1.0f));
            letterMap[workingIndex] = static_cast<int>(glyph->character);

            drawPosition.x += glyph->advance * scale;
            ++workingIndex;

            if (workingIndex == m_shader.maxInstances()) {
                renderFn();
                workingIndex = 0;
            }
        }

        if (workingIndex > 0) {
            renderFn();
        }
    }
} // namespace EconSimPlusPlus
