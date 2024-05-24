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
#include <EconSimPlusPlus/Engine/Glyph.hpp>
#include <EconSimPlusPlus/Engine/Shader.hpp>
#include <EconSimPlusPlus/Engine/TextureArray.hpp>
#include <EconSimPlusPlus/Engine/VertexArray.hpp>
#include <EconSimPlusPlus/Engine/VertexBuffer.hpp>

namespace EconSimPlusPlus::Engine {
    /// Handles the loading and rendering of TrueType fonts.
    class Font {

    public:
        // TODO: Move RenderSettings to own header file and give more descriptive name.
        /// The collection of settings for drawing text on screen.
        struct RenderSettings {
            /// The colour to render the text.
            glm::vec3 color{1.0f};
            /// The height, in pixels, at which to draw the text. Note that the width will be scaled proportionately to
            /// maintain the aspect ratio.
            float size{32.0f};
            /// The amount of horizontal and vertical space to add around the text measured in pixels.
            glm::vec2 padding{0.0f};
            /// The point on the text that the position refers to.
            Anchor anchor{Anchor::topLeft};
            /// The value between zero and one in the font SDF that indicates an edge.
            /// Increasing this value will shrink the font and increasing it will expand the font.
            float sdfThreshold{0.5f};
            /// The amount to smooth the edges as a ratio of the SDF range.
            float edgeSmoothness{0.01f};
            /// The size of the outline to draw around the edge of the text as a ratio of the SDF range. Set to zero to
            /// disable outlines.
            float outlineSize{0.0f};
            /// The colour of the text outline.
            glm::vec3 outlineColor{0.0f};
        };

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
        /// @param vao The vertex array for rendering text.
        /// @param vbo The vertex buffer for rendering text.
        /// @param textureArray The texture array that holds the textures for each glyph.
        /// @param fontSize The target size (width, height) of the glyphs in pixels.
        /// glyphs.
        explicit Font(std::map<char, std::unique_ptr<Glyph>>& glyphs, std::unique_ptr<VertexArray> vao,
                      std::unique_ptr<VertexBuffer> vbo, std::unique_ptr<TextureArray> textureArray,
                      glm::vec2 fontSize);

        /// Get the scale factor relative to the underlying bitmap size.
        /// @param settings The font settings to be used for rendering.
        /// @return The scale factor.
        [[nodiscard]] float calculateScaleFactor(const RenderSettings& settings) const;

        /// Draw text on screen.
        /// @param text The string to render.
        /// @param position Where to render the text in screen coordinates (pixels). Note that this corresponds to the
        /// bottom left corner of the text. The z-coordinate indicates the 'layer' to draw the text on.
        /// @param camera The camera to render the text with.
        /// @param settings The various settings that control the appearance of the rendered text.
        void render(std::string_view text, glm::vec3 position, const Camera& camera,
                    const RenderSettings& settings) const;

        /// Calculate the width and height of the text if it were rendered on screen.
        /// @note The size is for unscaled text.
        /// @param text The string to be rendered.
        /// @return The width and height of the text in pixels.
        [[nodiscard]] glm::vec2 calculateTextSize(std::string_view text) const;

    private:
        /// Mapping between ASCII chars (0-127) and the corresponding glyph data.
        std::map<char, std::unique_ptr<Glyph>> m_glyphs;
        /// The vertex array for storing the 3D geometry and atttributes.
        std::unique_ptr<VertexArray> m_vao;
        /// The vertex buffer for the text geometry.
        std::unique_ptr<VertexBuffer> m_vbo;
        /// The shader for rendering text via OpenGL.
        const Shader m_shader{Shader::create("resource/shader/text.vert", "resource/shader/text.frag")};

        /// The texture array that holds the textures for each glyph.
        const std::unique_ptr<TextureArray> m_textureArray;
        /// The target size (width, height) of the glyphs in pixels.
        const glm::vec2 m_fontSize;
    };
} // namespace EconSimPlusPlus::Engine


#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_FONT_HPP
