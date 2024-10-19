

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <EconSimPlusPlus/Camera.hpp>
#include <EconSimPlusPlus/Font.hpp>
#include <EconSimPlusPlus/Shader.hpp>
#include <EconSimPlusPlus/Quad.hpp>

namespace EconSimPlusPlus {

    /// A wrapper for commonly used rendering objects.
    struct Graphics {
        /// The camera used for rendering objects on screen.
        Camera camera;
        /// The default font used for rendering text.
        std::unique_ptr<Font> font{Font::create("resource/font/Roboto-Regular.ttf", {288, 288}, {64, 64}, 32.0f)};
        /// The shader intended for drawing a quad with an RGBA color.
        Shader quadShader{Shader::create("resource/shader/grid.vert", "resource/shader/rgba.frag")};
        // TODO: Create wrapper around quad and solid fill shader so that user only has to pass in camera + transform?
        /// A unit quad (width == height == 1 px) positioned at the world origin.
        Quad quad{};
    };

} // namespace EconSimPlusPlus

#endif // GRAPHICS_HPP
