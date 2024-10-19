

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_FONT_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_FONT_HPP

#include <map>

#include <EconSimPlusPlus/Anchor.hpp>
#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/Glyph.hpp>
#include <EconSimPlusPlus/Quad.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/TextureArray.hpp>

namespace EconSimPlusPlus {
    /// Handles the loading and rendering of TrueType fonts.
    class Font {
    public:
        /// The collection of settings for drawing text on screen.
        struct Style {
            /// The colour to render the text.
            glm::vec3 color{0.0f};
            /// The height, in pixels, at which to draw the text. Note that the width will be scaled proportionately to
            /// maintain the aspect ratio.
            float size{32.0f};
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
        /// @param textureArray The texture array that holds the textures for each glyph.
        /// @param fontSize The target size (width, height) of the glyphs in pixels.
        /// @param verticalExtents The maximum distance below and above the baseline, respectively.
        /// glyphs.
        explicit Font(std::map<char, std::unique_ptr<Glyph>>& glyphs, std::unique_ptr<TextureArray> textureArray,
                      glm::vec2 fontSize, glm::vec2 verticalExtents);

        Font(Font&) = delete; // Prevent issues with OpenGL stuff.

        /// Get the scale factor relative to the underlying bitmap size.
        /// @param style The font settings to be used for rendering.
        /// @return The scale factor.
        [[nodiscard]] float calculateScaleFactor(const Style& style) const;

        /// Calculate the width and height of the text if it were rendered on screen.
        /// @note The size is for unscaled text.
        /// @param text The string to be rendered.
        /// @return The width and height of the text in pixels.
        [[nodiscard]] glm::vec2 calculateTextSize(std::string_view text) const;

        /// Draw text on screen.
        /// @param text The string to render.
        /// @param position Where to render the text in screen coordinates (pixels). Note that this corresponds to the
        /// bottom left corner of the text. The z-coordinate indicates the 'layer' to draw the text on.
        /// @param anchor The point on the text that the position refers to.
        /// @param style The various settings that control the appearance of the rendered text.
        /// @param camera The camera to render the text with.
        void render(std::string_view text, glm::vec3 position, Anchor anchor, const Style& style,
                    const Camera& camera) const;

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
} // namespace EconSimPlusPlus


#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_FONT_HPP
