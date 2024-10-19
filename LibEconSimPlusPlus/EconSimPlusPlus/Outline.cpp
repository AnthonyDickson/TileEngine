

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Outline.hpp>


namespace EconSimPlusPlus::Outline {
    namespace {
        /// The minimum and maximum extents that define a rectangle.
        struct Rect {
            glm::vec2 bottomLeft;
            glm::vec2 topRight;
        };

        /// The rectangles that define an outline of an arbitrary thickness.
        struct Sides {
            Rect left;
            Rect right;
            Rect top;
            Rect bottom;
        };

        /// Calculate the coordinates of the outline geometry.
        /// @param object The GUI object to draw the outline around.
        /// @param outline The configuration for the outline appearance.
        /// @return The rectangles that define an outline.
        Sides getOutlineRectangles(const Object& object, const Outline::Style& outline) {
            const glm::vec2 origin{bottomLeft(object)};
            const float thickness{outline.thickness};
            const float leftEdge = origin.x;
            const float rightEdge = leftEdge + object.size().x;
            const float bottomEdge = origin.y;
            const float topEdge = bottomEdge + object.size().y;
            glm::vec2 bottomLeftCorner;
            glm::vec2 topRightCorner;

            Sides sides{};

            switch (outline.placement) {
            case Placement::inset:
                bottomLeftCorner = {leftEdge, bottomEdge};
                topRightCorner = {rightEdge, topEdge};

                sides.left = {bottomLeftCorner, {leftEdge + thickness, topEdge}};
                sides.right = {{rightEdge - thickness, bottomEdge}, topRightCorner};
                sides.bottom = {bottomLeftCorner, {rightEdge, bottomEdge + thickness}};
                sides.top = {{leftEdge, topEdge - thickness}, topRightCorner};
                break;
            case Placement::outset:
                bottomLeftCorner = glm::vec2{leftEdge, bottomEdge} - thickness;
                topRightCorner = glm::vec2{rightEdge, topEdge} + thickness;

                sides.left = {bottomLeftCorner, {leftEdge, topEdge + thickness}};
                sides.right = {{rightEdge, bottomEdge - thickness}, topRightCorner};
                sides.bottom = {bottomLeftCorner, {rightEdge + thickness, bottomEdge}};
                sides.top = {{leftEdge - thickness, topEdge}, topRightCorner};
                break;
            }

            return sides;
        }

        /// Draw one side of an outline.
        /// @note Assumes the shader has the uniform variables "transform" (glm::mat4) and "color" (glm::vec3).
        /// @note Assumes that the perspective/view matrices have already been set in the shader.
        /// @param object The GUI object to draw the outline around.
        /// @param shader The shader to draw the outline with.
        /// @param quad The quad geometry to use for drawing the outline.
        /// @param outline The configuration for the outline appearance.
        /// @param rect: The area to fill.
        void drawSide(const Object& object, const Shader& shader, const Quad& quad, const Style& outline,
                      const Rect& rect) {
            glm::mat4 transform = glm::translate(glm::mat4{1.0f}, {rect.bottomLeft, object.layer()});
            transform = glm::scale(transform, {rect.topRight - rect.bottomLeft, 1.0f});
            shader.setUniform("transform", transform);
            shader.setUniform("color", outline.color);
            quad.render();
        }
    } // namespace

    void draw(const Object& object, const Shader& shader, const Quad& quad, const Style& outline) {
        if (outline.thickness < 1.0f) {
            return;
        }

        const auto [left, right, top, bottom]{getOutlineRectangles(object, outline)};

        drawSide(object, shader, quad, outline, left);
        drawSide(object, shader, quad, outline, right);
        drawSide(object, shader, quad, outline, top);
        drawSide(object, shader, quad, outline, bottom);
    }
} // namespace EconSimPlusPlus::Outline
