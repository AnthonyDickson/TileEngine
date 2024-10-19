

#ifndef LIBTILEENGINE_TILEENGINE_GROUP_HPP
#define LIBTILEENGINE_TILEENGINE_GROUP_HPP

#include <TileEngine/Object.hpp>
#include <TileEngine/Outline.hpp>

namespace TileEngine {
    /// Automatically manages the layout of objects.
    class Group final : public Object {
    public:
        /// How objects in the group should be positioned relative to each other.
        enum class LayoutDirection { vertical, horizontal };
        /// How objects in the group should be horizontally positioned relative to the group.
        enum class HorizontalAlignment { left, center, right, justified };
        /// How objects in the group should be vertically positioned relative to the group.
        enum class VerticalAlignment { top, center, bottom, justified };

        /// Configuration for group layout.
        struct Layout {
            /// How objects in the group should be positioned relative to each other.
            LayoutDirection direction{LayoutDirection::vertical};
            /// The horizontal and vertical space between the inner borders of the group and its child objects in
            /// pixels.
            glm::vec2 padding{};
            /// The space between child objects in pixels.
            float spacing{};
            /// How objects in the group should be horizontally positioned relative to the group.
            /// @note Only has an effect if the group size is set to something larger than the minimal containing size.
            HorizontalAlignment horizontalAlignment{HorizontalAlignment::left};
            /// How objects in the group should be vertically positioned relative to the group.
            /// @note Only has an effect if the group size is set to something larger than the minimal containing size.
            VerticalAlignment verticalAlignment{VerticalAlignment::top};
        };

        /// The configuration for the appearance of a group.
        /// @note By default, the group itself does not render anything, only its child objects.
        struct Style {
            /// The color to fill the area of the group with.
            std::optional<glm::vec4> fillColor;
            /// The outline to draw around the group.
            std::optional<Outline::Style> outline;
        };

        /// Create an empty group.
        /// @param layout Configuration for group layout.
        /// @param style Configuration for group appearance.
        explicit Group(const Layout& layout, const Style& style = Style{});

        void setPosition(glm::vec2 position) override;
        void setLayer(float layer) override;
        void setSize(glm::vec2 size) override;
        void addChild(const std::shared_ptr<Object>& object) override;

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Graphics& graphics) const override;

    private:
        /// Recalculate the group layout from scratch.
        /// @note Intended to be called after the group's position is updated or an object is added so that objects are
        /// positioned correctly.
        void recalculateLayout();

        /// Configuration for group layout.
        const Layout m_layout;
        /// Configuration for group appearance.
        const Style m_style;
    };

    /// Calculate the size of the group that would contain all objects plus padding and spacing.
    /// @param objects A list of objects that the group contains.
    /// @param style The group style containing the padding and spacing parameters.
    /// @return The width and height in pixels.
    glm::vec2 calculateContainingSize(const std::vector<std::shared_ptr<Object>>& objects, const Group::Layout& style);

} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_GROUP_HPP
