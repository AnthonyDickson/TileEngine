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
#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {

    // TODO: Try out Valve's approach to high-res scalable text from signed distance fields:
    // https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf
    /// Handles the loading and rendering of TrueType fonts.
    class Font {
    private:
        /// Mapping between ASCII chars (0-127) and the corresponding glyph data.
        std::map<char, std::unique_ptr<Glyph>> glyphs;
        /// The vertex array for storing the 3D geometry and atttributes.
        std::unique_ptr<VertexArray> vao;
        /// The vertex buffer for the text geometry.
        std::unique_ptr<VertexBuffer> vbo;
        /// The shader for rendering text via OpenGL.
        Shader shader{"resource/shader/text.vert", "resource/shader/text.frag"};

    public:
        /// The point in the text that the render position refers to.
        enum class Anchor { bottomLeft, bottomRight, topLeft, topRight, center };

        /// Create a font (collection of glyphs).
        /// @param glyphs_ Mapping between ASCII chars and the corresponing font data.
        /// @param vao_ The vertex array for rendering text.
        /// @param vbo_ The vertex buffer for rendering text.
        explicit Font(std::map<char, std::unique_ptr<Glyph>>& glyphs_, std::unique_ptr<VertexArray> vao_,
                      std::unique_ptr<VertexBuffer> vbo_);

        /// Create a font object from a TrueType font.
        /// @param fontPath The path to the TrueType font file on disk.
        /// @return A font object.
        static std::unique_ptr<Font> create(const std::string& fontPath);

        /// Draw text on screen.
        /// @param text The string to render.
        /// @param position Where to render the text in screen coordinates (pixels). Note that this corresponds to the
        /// bottom left corner of the text.
        /// @param scale How much to scale up or down the text from its original size.
        /// @param colour The colour to render the text.
        /// @param camera The camera to render the text with.
        /// @param anchor The point on the text that the position refers to.
        void render(std::string_view text, glm::vec2 position, float scale, glm::vec3 colour, const Camera& camera,
                    Anchor anchor = Anchor::bottomLeft) const;

    private:
        /// Calculate the positional offset for the given text and anchor.
        /// @note Assumes single-line text.
        /// @param text The string to be rendered.
        /// @param anchor The reference point from which the text will be rendered.
        /// @param scale The scale factor to be applied to the rendered text.
        /// @return A 2D offset in screen coordinates.
        glm::vec2 calculateAnchorOffset(std::string_view text, Anchor anchor, float scale) const;
    };
} // namespace EconSimPlusPlus


#endif // FONT_HPP
