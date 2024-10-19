

//
// Created by Anthony Dickson on 28/06/2024.

#include "glm/ext/matrix_transform.hpp"

#include <TileEngine/Group.hpp>

namespace TileEngine {
    namespace {
        /// Calculate where to place the first object of a group based on the group's layout.
        /// @param group The group to calculate the position relative to.
        /// @param layout The group layout config.
        /// @param innerSize The size (width, height) of the group minus padding in pixels.
        /// @param contentSize The size (width, height) of the group's child objects plus spacing in pixels.
        /// @return The position (x, y) of the first object in pixels.
        glm::vec2 calculateInitialPosition(const Group& group, const Group::Layout& layout, const glm::vec2 innerSize,
                                           const glm::vec2 contentSize) {
            glm::vec2 initialPosition{0.0f};

            switch (layout.horizontalAlignment) {
            case Group::HorizontalAlignment::left:
            case Group::HorizontalAlignment::justified:
                initialPosition.x = left(group) + 0.5f * layout.padding.x;
                break;
            case Group::HorizontalAlignment::right:
                initialPosition.x = left(group) + 0.5f * layout.padding.x + (innerSize.x - contentSize.x);
                break;
            case Group::HorizontalAlignment::center:
                initialPosition.x = left(group) + 0.5f * layout.padding.x + 0.5f * (innerSize.x - contentSize.x);
                break;
            }

            switch (layout.verticalAlignment) {
            case Group::VerticalAlignment::top:
            case Group::VerticalAlignment::justified:
                initialPosition.y = top(group) - 0.5f * layout.padding.y;
                break;
            case Group::VerticalAlignment::bottom:
                initialPosition.y = top(group) - 0.5f * layout.padding.y - (innerSize.y - contentSize.y);
                break;
            case Group::VerticalAlignment::center:
                initialPosition.y = top(group) - 0.5f * layout.padding.y - 0.5f * (innerSize.y - contentSize.y);
                break;
            }

            return initialPosition;
        }

        /// Calculate the offset position for an object.
        ///
        /// In groups with objects of different sizes, we need to calculate an offset to ensure that all objects are
        /// aligned correctly, irregardless of width/height.
        /// @param object The object to calculate the position for.
        /// @param basePosition The position for the standard
        /// @param layout The group layout config.
        /// @param innerSize The size (width, height) of the group minus padding in pixels.
        /// @return The position (x, y) of the object in pixels.
        glm::vec2 calculatePosition(const std::shared_ptr<Object>& object, const glm::vec2 basePosition,
                                    const Group::Layout& layout, const glm::vec2 innerSize) {
            glm::vec2 centeringOffset{0.0f};

            if (layout.direction == Group::LayoutDirection::horizontal) {
                switch (layout.verticalAlignment) {
                case Group::VerticalAlignment::center:
                    centeringOffset.y -= 0.5f * (innerSize.y - object->size().y);
                    break;
                case Group::VerticalAlignment::bottom:
                    centeringOffset.y -= innerSize.y - object->size().y;
                    break;
                default:
                    break;
                }
            }

            if (layout.direction == Group::LayoutDirection::vertical) {
                switch (layout.horizontalAlignment) {
                case Group::HorizontalAlignment::center:
                    centeringOffset.x += 0.5f * (innerSize.x - object->size().x);
                    break;
                case Group::HorizontalAlignment::right:
                    centeringOffset.x += innerSize.x - object->size().x;
                    break;
                default:
                    break;
                }
            }

            return glm::vec2{basePosition + centeringOffset};
        }

        /// Calculate where to place the next object in a group.
        /// @param object The object most recently positioned.
        /// @param currentPosition The position of the most recently positioned object.
        /// @param layout The group layout config.
        /// @param innerSize The size (width, height) of the group minus padding in pixels.
        /// @param contentSize The size (width, height) of the group's child objects plus spacing in pixels.
        /// @param itemCount The number of objects in the group.
        /// @return The position (x, y) of the next object in pixels.
        glm::vec2 calculateNextPosition(const std::shared_ptr<Object>& object, const glm::vec2 currentPosition,
                                        const Group::Layout& layout, const glm::vec2 innerSize,
                                        const glm::vec2 contentSize, const int itemCount) {
            glm::vec2 nextPosition{currentPosition};

            switch (layout.direction) {
            case Group::LayoutDirection::horizontal:
                switch (layout.horizontalAlignment) {
                case Group::HorizontalAlignment::left:
                case Group::HorizontalAlignment::center:
                case Group::HorizontalAlignment::right:
                    nextPosition.x = currentPosition.x + object->size().x + layout.spacing;
                    break;
                case Group::HorizontalAlignment::justified:
                    nextPosition.x = currentPosition.x + object->size().x + layout.spacing +
                        (innerSize.x - contentSize.x) / (static_cast<float>(itemCount) - 1.0f);
                    break;
                }
                break;
            case Group::LayoutDirection::vertical:
                switch (layout.verticalAlignment) {
                case Group::VerticalAlignment::top:
                case Group::VerticalAlignment::center:
                case Group::VerticalAlignment::bottom:
                    nextPosition.y = currentPosition.y - object->size().y - layout.spacing;
                    break;
                case Group::VerticalAlignment::justified:
                    nextPosition.y = currentPosition.y - object->size().y - layout.spacing -
                        (innerSize.y - contentSize.y) / (static_cast<float>(itemCount) - 1.0f);
                    break;
                }
                break;
            default:
                throw std::runtime_error("Unsupported layout.");
            }

            return nextPosition;
        }
    } // namespace
    Group::Group(const Layout& layout, const Style& style) : m_layout(layout), m_style(style) {
    }

    void Group::setPosition(const glm::vec2 position) {
        const glm::vec2 delta{position - this->position()};
        Object::setPosition(position);

        for (const auto& object : children()) {
            object->setPosition(object->position() + delta);
        }
    }

    void Group::setLayer(const float layer) {
        Object::setLayer(layer);

        for (const auto& object : children()) {
            object->setLayer(layer);
        }
    }

    void Group::setSize(const glm::vec2 size) {
        Object::setSize(size);
        recalculateLayout();
    }

    void Group::addChild(const std::shared_ptr<Object>& object) {
        Object::addChild(object);

        if (const glm::vec2 containingSize{calculateContainingSize(children(), m_layout)};
            glm::any(glm::greaterThan(containingSize, size()))) {
            setSize(glm::vec2{std::max(containingSize.x, size().x), std::max(containingSize.y, size().y)});
        }

        recalculateLayout();
    }

    void Group::update(const float deltaTime, const InputState& inputState, const Camera& camera) {
        for (const std::shared_ptr<Object>& object : children()) {
            object->update(deltaTime, inputState, camera);
        }
    }

    void Group::render(const Graphics& graphics) const {
        if (m_style.fillColor.has_value()) {
            graphics.quadShader.bind();
            graphics.quadShader.setUniform("projectionViewMatrix", projectionViewMatrix(graphics.camera));
            glm::mat4 transform{glm::translate(glm::mat4{1.0f}, {bottomLeft(*this), layer()})};
            transform = glm::scale(transform, {size(), 1.0f});
            graphics.quadShader.setUniform("transform", transform);
            graphics.quadShader.setUniform("color", *m_style.fillColor);
            graphics.quad.render();
        }

        if (m_style.outline.has_value()) {
            graphics.quadShader.bind();
            graphics.quadShader.setUniform("projectionViewMatrix", projectionViewMatrix(graphics.camera));
            Outline::draw(*this, graphics.quadShader, graphics.quad, *m_style.outline);
        }

        for (const std::shared_ptr<Object>& object : children()) {
            object->render(graphics);
        }
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void Group::recalculateLayout() {
        // Cache child objects since we loop over this collection multiple times.
        const auto childObjects{children()};

        // The size of all child objects plus padding and spacing.
        const glm::vec2 containingSize{calculateContainingSize(childObjects, m_layout)};
        // The size of all child objects lined up with the layout defined spacing between them.
        const glm::vec2 contentSize{containingSize - m_layout.padding};

        // The group size including padding.
        const glm::vec2 outerSize{size()};
        // The group size excluding padding.
        const glm::vec2 innerSize{outerSize - m_layout.padding};

        glm::vec2 nextPosition{calculateInitialPosition(*this, m_layout, innerSize, contentSize)};

        for (std::size_t i = 0; i < childObjects.size(); ++i) {
            const auto& object{childObjects.at(i)};
            object->setAnchor(Anchor::topLeft);

            if (childObjects.size() > 1) {
                // TODO: Make alignment along layout direction axis optional.
                object->setPosition(calculatePosition(object, nextPosition, m_layout, innerSize));
            }
            else {
                object->setPosition(nextPosition);
            }

            object->setLayer(layer());

            nextPosition = calculateNextPosition(object, nextPosition, m_layout, innerSize, contentSize,
                                                 static_cast<int>(childObjects.size()));
        }
    }

    glm::vec2 calculateContainingSize(const std::vector<std::shared_ptr<Object>>& objects, const Group::Layout& style) {
        glm::vec2 containingSize{0.0f};

        for (const std::shared_ptr<Object>& object : objects) {
            switch (style.direction) {
            case Group::LayoutDirection::horizontal:
                containingSize.x = std::max(containingSize.x, containingSize.x + object->size().x);
                containingSize.y = std::max(containingSize.y, object->size().y);
                break;
            case Group::LayoutDirection::vertical:
                containingSize.x = std::max(containingSize.x, object->size().x);
                containingSize.y = std::max(containingSize.y, containingSize.y + object->size().y);
                break;
            default:
                throw std::runtime_error("Unsupported layout.");
            }
        }

        switch (style.direction) {
        case Group::LayoutDirection::horizontal:
            containingSize.x += style.spacing * (static_cast<float>(objects.size()) - 1.0f);
            break;
        case Group::LayoutDirection::vertical:
            containingSize.y += style.spacing * (static_cast<float>(objects.size()) - 1.0f);
            break;
        default:
            throw std::runtime_error("Unsupported layout.");
        }

        containingSize += style.padding;

        return containingSize;
    }
} // namespace TileEngine
