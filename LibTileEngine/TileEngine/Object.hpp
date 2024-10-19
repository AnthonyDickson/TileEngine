

#ifndef LIBTILEENGINE_TILEENGINE_OBJECT_HPP
#define LIBTILEENGINE_TILEENGINE_OBJECT_HPP

#include <functional>

#include "glm/ext/matrix_common.hpp"
#include "glm/vec2.hpp"

#include <TileEngine/Anchor.hpp>
#include <TileEngine/Graphics.hpp>
#include <TileEngine/Event.hpp>
#include <TileEngine/InputState.hpp>

namespace TileEngine {

    class Object {
    public:
        virtual ~Object();

        /// Get whether the object should be able to receive 'focus'.
        /// @return `true` if this object can receive focus.
        [[nodiscard]] bool focusable() const;

        /// Set whether the object should be able to receive 'focus'.
        /// @param value `true` if the object should receive focus, otherwise `false`.
        void setFocusable(bool value);

        /// Get the object's 2D coordinate in world space.
        /// @return A 2-vector.
        [[nodiscard]] glm::vec2 position() const;

        /// Set the 2D coordinates of an object.
        /// @param position The object's 2D coordinate in world space.
        virtual void setPosition(glm::vec2 position);

        /// The layer the object exists on.
        /// This controls the ordering during rendering. Higher values put the object closer to the camera.
        /// @return A non-negative number.
        [[nodiscard]] float layer() const;

        /// Set the object's layer.
        /// @param layer The layer the object exists on. Higher values put the object closer to the camera.
        virtual void setLayer(float layer);

        /// Get the object's dimensions.
        /// @return A 2-vector.
        [[nodiscard]] glm::vec2 size() const;

        /// Set the size of the object.
        /// @param size The 2D dimensions of the object in pixels.
        virtual void setSize(glm::vec2 size);

        /// Get the point on the object that is used as the origin for its position.
        /// @return An anchor.
        [[nodiscard]] Anchor anchor() const;

        /// Set the object's anchor point.
        /// @param anchor The point on the object that is used as the origin for its position.
        virtual void setAnchor(Anchor anchor);

        /// Get the objects contained by this object.
        /// @return A list of object pointers.
        [[nodiscard]] const std::vector<std::shared_ptr<Object>>& children() const;

        /// Add an object.
        /// @param object An object.
        virtual void addChild(const std::shared_ptr<Object>& object);

        /// Remove an object.
        /// @param object An object.
        virtual void removeChild(const std::shared_ptr<Object>& object);

        /// Register an event handler for this object.
        /// @param eventHandler A function that responds to events.
        void addEventHandler(const EventHandler& eventHandler);

        /// Tell the object that an event has occured.
        /// @param event The event that has occured.
        /// @param eventData Information about the event.
        void notify(Event event, const EventData& eventData);

        /// Update the object.
        /// @param deltaTime The size of the step to take in terms of time (seconds).
        /// @param inputState The state of keyboard and mouse input.
        /// @param camera The main camera. Can be used to convert between screen space and world space coordinates.
        virtual void update(float deltaTime, const InputState& inputState, const Camera& camera) = 0;

        /// Draw the object on screen.
        /// @param graphics The graphics object to render the tile map with.
        virtual void render(const Graphics& graphics) const = 0;

    private:
        /// The world space coordinates of the object along with the layer. Note this refers to the top left of the
        /// object.
        glm::vec3 m_position{0.0f};
        /// The width and height of the object in world space coordinates.
        glm::vec2 m_size{1.0f};
        /// The point on the object that is used as the origin for its position.
        Anchor m_anchor{Anchor::topLeft};
        /// Whether the object should be able to receive 'focus' (e.g., by being clicked on) and become the sole
        /// receiver of keyboard input. This is primarily useful for text fields.
        bool m_focusable{false};
        /// The registered event handlers.
        std::vector<EventHandler> m_eventHandlers{};
        /// The objects contained by this object.
        std::vector<std::shared_ptr<Object>> m_children{};
    };

    /// Traverse a list of objects recursively.
    /// @param objects The list of objects to traverse.
    /// @return The flattened list of the objects pointers in depth-first post-order.
    /// @note Post-order traversal is used to avoid container objects (e.g., `Group`) from blocking mouse events from
    /// reaching the child objects.
    std::vector<std::shared_ptr<Object>> traverse(const std::vector<std::shared_ptr<Object>>& objects);

    /// Check whether a point is contained in the object's axis-aligned bounding box.
    /// @param object An object.
    /// @param point The 2D point to test.
    /// @return Whether the point interects the object.
    [[nodiscard]] bool contains(const Object& object, glm::vec2 point);

    /// Get the coordinates of the top edge of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] float top(const Object& object);

    /// Get the coordinates of the bottom edge of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] float bottom(const Object& object);

    /// Get the coordinates of the left edge of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] float left(const Object& object);

    /// Get the coordinates of the right edge of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] float right(const Object& object);

    /// Get the coordinates of the top left corner of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] glm::vec2 topLeft(const Object& object);

    /// Get the coordinates of the top right corner of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] glm::vec2 topRight(const Object& object);

    /// Get the coordinates of the bottom left corner of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] glm::vec2 bottomLeft(const Object& object);

    /// Get the coordinates of the bottom right corner of the object.
    /// @param object An object.
    /// @return The object's 2D coordinate in world space.
    [[nodiscard]] glm::vec2 bottomRight(const Object& object);

} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_OBJECT_HPP
