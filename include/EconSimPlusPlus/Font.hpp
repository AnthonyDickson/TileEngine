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

#include <EconSimPlusPlus/Glyph.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/VertexArray.hpp>
#include <EconSimPlusPlus/VertexBuffer.hpp>

namespace EconSimPlusPlus {

    // TODO: Add docstrings
    /// Handles the loading and rendering of TrueType fonts.
    class Font {
    private:
        std::map<char, std::unique_ptr<Glyph>> glyphs;
        std::unique_ptr<VertexArray> vao;
        std::unique_ptr<VertexBuffer> vbo;
        Shader shader{"resource/shader/text.vert", "resource/shader/text.frag"};

    public:
        explicit Font(std::map<char, std::unique_ptr<Glyph>>& glyphs_, std::unique_ptr<VertexArray> vao_,
                      std::unique_ptr<VertexBuffer> vbo_);

        static std::unique_ptr<Font> create(const std::string& fontPath);

        void render(std::string_view text, glm::vec2 position, float scale, glm::vec3 colour) const;
    };
} // namespace EconSimPlusPlus


#endif // FONT_HPP
