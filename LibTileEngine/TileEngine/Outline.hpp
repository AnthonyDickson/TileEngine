

#ifndef LIBTILEENGINE_TILEENGINE_OUTLINE_HPP
#define LIBTILEENGINE_TILEENGINE_OUTLINE_HPP


#include <TileEngine/Object.hpp>
#include <TileEngine/Quad.hpp>
#include <TileEngine/Shader.hpp>


namespace TileEngine::Outline {
    /// Where to draw the outline.
    enum class Placement { inset, outset };

    /// The configuration for the appearance of an outline.
    struct Style {
        /// The color of the outline.
        glm::vec4 color;
        /// The thickness of the outline in pixels.
        float thickness;
        /// Where to draw the outline.
        Placement placement;
    };

    /// Draw an outline around a GUI object.
    /// @note Assumes the shader has the uniform variables "transform" (glm::mat4) and "color" (glm::vec3).
    /// @note Assumes that the perspective/view matrices have already been set in the shader.
    /// @note Does not draw anything if the outline thickness is less than one pixel.
    /// @param object The GUI object to draw the outline around.
    /// @param shader The shader to draw the outline with.
    /// @param quad The quad geometry to use for drawing the outline.
    /// @param outline The configuration for the outline appearance.
    void draw(const Object& object, const Shader& shader, const Quad& quad, const Style& outline);

} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_OUTLINE_HPP
