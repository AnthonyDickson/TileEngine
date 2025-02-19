

//
// Created by Anthony on 26/03/2024.
//

#ifndef LIBTILEENGINE_TILEENGINE_CAMERA_HPP
#define LIBTILEENGINE_TILEENGINE_CAMERA_HPP

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/vec3.hpp"

#include <TileEngine/Direction.hpp>
#include <TileEngine/InputState.hpp>
#include <TileEngine/Viewport.hpp>

namespace TileEngine {

    /// An object used to manipulate the view of a scene in OpenGL.
    class Camera {

    public:
        Camera(glm::vec2 viewport, glm::vec3 position);

        /// Get the perspective matrix of the camera.
        /// @return A 4x4 matrix.
        [[nodiscard]] glm::mat4 perspectiveMatrix() const;

        /// Get the view matrix of the camera.
        /// @return A 4x4 matrix.
        [[nodiscard]] glm::mat4 viewMatrix() const;

        /// Get the camera position.
        /// @return A 3-vector position in world coordinates.
        [[nodiscard]] glm::vec3 position() const;

        /// Get the viewport size.
        /// @return the width and height in pixels.
        [[nodiscard]] glm::vec2 viewportSize() const;

        /// Get the visible area from the camera after all transforms.
        /// @return The viewport extents.
        [[nodiscard]] Viewport viewport() const;

        /// Update the camera.
        /// @param deltaTime The size of the step to take in terms of time (seconds).
        /// @param inputState The state of keyboard and mouse input.
        void update(float deltaTime, const InputState& inputState);

        /// Move/translate the camera.
        /// @param direction The direction to move the camera in.
        /// @param speed How fast to move the camera.
        void move(Direction direction, float speed);

        /// Moves the camera back to origin.
        void resetPosition();

        /// Update the camera to match the window size when the user resizes the window.
        /// @param viewport The new window size.
        void onWindowResize(glm::vec2 viewport);

    private:
        /// The dimensions (width, height in pixels) of the window viewport and therefore the camera viewport.
        glm::vec2 m_viewport;
        /// The position of the camera in world space.
        glm::vec3 m_position;
        /// The projection matrix (e.g., perspective or orthographic).
        glm::mat4 m_projection;
    };

    /// Convert screen space coordinates to world space coordinates.
    /// @param screenCoordinates Coordinates in screen space starting the from the top left with +y pointing down.
    /// @param camera The camera to use for the coordinate space conversion.
    /// @return 2D world coordinates.
    /// @note The centre of the viewport is mapped to the origin for the world space coordinates and a
    /// right-handed coordinate system is used.
    glm::vec2 screenToWorldCoordinates(glm::vec2 screenCoordinates, const Camera& camera);

    /// Get a copy of the camera positioned at the world origin.
    /// @param camera The camera to copy.
    /// @return A camera at the world origin.
    [[nodiscard]] Camera atOrigin(const Camera& camera);

    /// Calculate the product of a camera's projection and view matrices.
    /// @param camera A camera.
    /// @return A homogeneous 4x4 transformation matrix.
    /// @note The product of this matrix and an object's model matrix projects the object onto the image plane of the
    /// camera.
    [[nodiscard]] glm::mat4 projectionViewMatrix(const Camera& camera);
} // namespace TileEngine


#endif // LIBTILEENGINE_TILEENGINE_CAMERA_HPP
