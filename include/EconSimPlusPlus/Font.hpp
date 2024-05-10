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

#ifndef FONT_HPP
#define FONT_HPP

#include <map>

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/Glyph.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/TextureArray.hpp>
#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {

    /// Handles the loading and rendering of TrueType fonts.
    class Font {
        /// Mapping between ASCII chars (0-127) and the corresponding glyph data.
        std::map<char, std::unique_ptr<Glyph>> glyphs;
        /// The vertex array for storing the 3D geometry and atttributes.
        std::unique_ptr<VertexArray> vao;
        /// The vertex buffer for the text geometry.
        std::unique_ptr<VertexBuffer> vbo;
        /// The shader for rendering text via OpenGL.
        const Shader shader{"resource/shader/text.vert", "resource/shader/text.frag"};

        /// The texture array that holds the textures for each glyph.
        const std::unique_ptr<TextureArray> textureArray;
        /// The target size (width, height) of the glyphs in pixels.
        const glm::vec2 fontSize;
        /// The range of ASCII characters to generate glyphs for.
        static constexpr int charsToGenerate{128};

    public:
        /// The point in the text that the render position refers to.
        enum class Anchor { bottomLeft, bottomRight, topLeft, topRight, center };

        /// The collection of settings for drawing text on screen.
        struct RenderSettings {
            /// The colour to render the text.
            glm::vec3 color{1.0f};
            /// The height, in pixels, at which to draw the text. Note that the width will be scaled proportionately to
            /// maintain the aspect ratio.
            float size{32.0f};
            /// The point on the text that the position refers to.
            Font::Anchor anchor{Font::Anchor::topLeft};
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

        /// Create a font (collection of glyphs).
        /// @param glyphs_ Mapping between ASCII chars and the corresponing font data.
        /// @param vao_ The vertex array for rendering text.
        /// @param vbo_ The vertex buffer for rendering text.
        /// @param textureArray_ The texture array that holds the textures for each glyph.
        /// @param fontSize_ The target size (width, height) of the glyphs in pixels.
        /// glyphs.
        explicit Font(std::map<char, std::unique_ptr<Glyph>>& glyphs_, std::unique_ptr<VertexArray> vao_,
                      std::unique_ptr<VertexBuffer> vbo_, std::unique_ptr<TextureArray> textureArray_,
                      glm::vec2 fontSize_);

        /// Create a Signed Distance Field (SDF) font object from a TrueType font.
        /// @param fontPath The path to the TrueType font file on disk.
        /// @param sdfFontSize The width and height in pixels of the fonts to use for generating the SDFs.
        /// @param textureSize The width and height in pixels of the final glyph textures.
        /// @param spread A scaling factor that the SDF values are divided by. Larger values scale up the size of text
        /// effects such as outlines and drop shadows.
        /// @return A font object.
        static std::unique_ptr<Font> create(const std::string& fontPath, glm::ivec2 sdfFontSize = {512, 512},
                                            glm::ivec2 textureSize = {64, 64}, float spread = 8.0f);

        /// Draw text on screen.
        /// @param text The string to render.
        /// @param position Where to render the text in screen coordinates (pixels). Note that this corresponds to the
        /// bottom left corner of the text. The z-coordinate indicates the 'layer' to draw the text on.
        /// @param camera The camera to render the text with.
        /// @param settings
        void render(std::string_view text, glm::vec3 position, const Camera& camera,
                    const RenderSettings& settings) const;

    private:
        /// Calculate the positional offset for the given text and anchor.
        /// @note The offset is for unscaled text.
        /// @param text The string to be rendered.
        /// @param anchor The reference point from which the text will be rendered.
        /// @return A 2D offset in screen coordinates.
        [[nodiscard]] glm::vec2 calculateAnchorOffset(std::string_view text, Anchor anchor) const;

        /// Calculate the width and height of the text if it were rendered on screen.
        /// @note The size is for unscaled text.
        /// @param text The string to be rendered.
        /// @return The width and height of the text in pixels.
        [[nodiscard]] glm::vec2 calculateTextSize(std::string_view text) const;
    };
} // namespace EconSimPlusPlus


#endif // FONT_HPP
