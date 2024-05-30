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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_FONT_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_FONT_HPP

#include <map>

#include <EconSimPlusPlus/Engine/Anchor.hpp>
#include <EconSimPlusPlus/Engine/Camera.hpp>
#include <EconSimPlusPlus/Engine/FontSettings.hpp>
#include <EconSimPlusPlus/Engine/Glyph.hpp>
#include <EconSimPlusPlus/Engine/Quad.hpp>
#include <EconSimPlusPlus/Engine/Shader.hpp>
#include <EconSimPlusPlus/Engine/TextureArray.hpp>

namespace EconSimPlusPlus::Engine {
    /// Handles the loading and rendering of TrueType fonts.
    class Font {

    public:
        /// Create a Signed Distance Field (SDF) font object from a TrueType font.
        /// @param fontPath The path to the TrueType font file on disk.
        /// @param sdfFontSize The width and height in pixels of the fonts to use for generating the SDFs.
        /// @param textureSize The width and height in pixels of the final glyph textures.
        /// @param spread A scaling factor that the SDF values are divided by. Larger values scale up the size of text
        /// effects such as outlines and drop shadows.
        /// @return A font object.
        static std::unique_ptr<Font> create(const std::string& fontPath, glm::ivec2 sdfFontSize = {512, 512},
                                            glm::ivec2 textureSize = {64, 64}, float spread = 8.0f);

        /// Create a font (collection of glyphs).
        /// @param glyphs Mapping between ASCII chars and the corresponing font data.
        /// @param textureArray The texture array that holds the textures for each glyph.
        /// @param fontSize The target size (width, height) of the glyphs in pixels.
        /// @param verticalExtents The maximum distance below and above the baseline, respectively.
        /// glyphs.
        explicit Font(std::map<char, std::unique_ptr<Glyph>>& glyphs, std::unique_ptr<TextureArray> textureArray,
                      glm::vec2 fontSize, glm::vec2 verticalExtents);

        Font(Font&) = delete; // Prevent issues with OpenGL stuff.

        /// Get the scale factor relative to the underlying bitmap size.
        /// @param settings The font settings to be used for rendering.
        /// @return The scale factor.
        [[nodiscard]] float calculateScaleFactor(const FontSettings& settings) const;

        /// Calculate the width and height of the text if it were rendered on screen.
        /// @note The size is for unscaled text.
        /// @param text The string to be rendered.
        /// @return The width and height of the text in pixels.
        [[nodiscard]] glm::vec2 calculateTextSize(std::string_view text) const;

        /// Draw text on screen.
        /// @param text The string to render.
        /// @param position Where to render the text in screen coordinates (pixels). Note that this corresponds to the
        /// bottom left corner of the text. The z-coordinate indicates the 'layer' to draw the text on.
        /// @param camera The camera to render the text with.
        /// @param settings The various settings that control the appearance of the rendered text.
        void render(std::string_view text, glm::vec3 position, const Camera& camera,
                    const FontSettings& settings) const;

    private:
        /// Mapping between ASCII chars (0-127) and the corresponding glyph data.
        std::map<char, std::unique_ptr<Glyph>> m_glyphs;
        /// The target size (width, height) of the glyphs in pixels.
        const glm::vec2 m_fontSize;
        /// The maximum distance below and above the baseline, respectively. The distance below the baseline is stored
        /// as a negative number.
        const glm::vec2 m_verticalExtents;

        /// The underlying geometry for drawing glyphs.
        Quad m_quad{};
        /// The shader for rendering text via OpenGL.
        const Shader m_shader{Shader::create("resource/shader/text.vert", "resource/shader/text.frag")};
        /// The texture array that holds the textures for each glyph.
        const std::unique_ptr<TextureArray> m_textureArray;
    };
} // namespace EconSimPlusPlus::Engine


#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_FONT_HPP
