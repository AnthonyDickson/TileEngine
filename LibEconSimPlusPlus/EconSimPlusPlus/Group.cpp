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
// Created by Anthony Dickson on 28/06/2024.

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Group.hpp>

namespace EconSimPlusPlus {
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

        glm::vec2 nextPosition{};

        switch (m_layout.horizontalAlignment) {
        case HorizontalAlignment::left:
        case HorizontalAlignment::justified:
            nextPosition.x = left(*this) + 0.5f * m_layout.padding.x;
            break;
        case HorizontalAlignment::right:
            nextPosition.x = left(*this) + 0.5f * m_layout.padding.x + (innerSize.x - contentSize.x);
            break;
        case HorizontalAlignment::center:
            nextPosition.x = left(*this) + 0.5f * m_layout.padding.x + 0.5f * (innerSize.x - contentSize.x);
            break;
        }

        switch (m_layout.verticalAlignment) {
        case VerticalAlignment::top:
        case VerticalAlignment::justified:
            nextPosition.y = top(*this) - 0.5f * m_layout.padding.y;
            break;
        case VerticalAlignment::bottom:
            nextPosition.y = top(*this) - 0.5f * m_layout.padding.y - (innerSize.y - contentSize.y);
            break;
        case VerticalAlignment::center:
            nextPosition.y = top(*this) - 0.5f * m_layout.padding.y + 0.5f * (innerSize.y - contentSize.y);
            break;
        }

        std::vector<glm::vec2> positions{};

        for (auto& object : childObjects) {
            glm::vec2 centeringOffset{0.0f};

            if (m_layout.direction == LayoutDirection::horizontal) {
                switch (m_layout.verticalAlignment) {
                case VerticalAlignment::center:
                    centeringOffset.y -= 0.5f * (innerSize.y - object->size().y);
                    break;
                case VerticalAlignment::bottom:
                    centeringOffset.y -= innerSize.y - object->size().y;
                    break;
                default:
                    break;
                }
            }

            if (m_layout.direction == LayoutDirection::vertical) {
                switch (m_layout.horizontalAlignment) {
                case HorizontalAlignment::center:
                    centeringOffset.x += 0.5f * (innerSize.x - object->size().x);
                    break;
                case HorizontalAlignment::right:
                    centeringOffset.x += innerSize.x - object->size().x;
                    break;
                default:
                    break;
                }
            }

            const glm::vec2 objectTopLeft{nextPosition + centeringOffset};
            positions.push_back(objectTopLeft);

            switch (m_layout.direction) {
            case LayoutDirection::horizontal:
                switch (m_layout.horizontalAlignment) {
                case HorizontalAlignment::left:
                case HorizontalAlignment::center:
                case HorizontalAlignment::right:
                    nextPosition.x = objectTopLeft.x + object->size().x + m_layout.spacing;
                    break;
                case HorizontalAlignment::justified:
                    nextPosition.x = objectTopLeft.x + object->size().x + m_layout.spacing +
                        (innerSize.x - contentSize.x) / (static_cast<float>(childObjects.size()) - 1.0f);
                    break;
                }
                break;
            case LayoutDirection::vertical:
                switch (m_layout.verticalAlignment) {
                case VerticalAlignment::top:
                case VerticalAlignment::center:
                case VerticalAlignment::bottom:
                    nextPosition.y = objectTopLeft.y - object->size().y - m_layout.spacing;
                    break;
                case VerticalAlignment::justified:
                    nextPosition.y = objectTopLeft.y - object->size().y - m_layout.spacing -
                        (innerSize.y - contentSize.y) / (static_cast<float>(childObjects.size()) - 1.0f);
                    break;
                }
                break;
            default:
                throw std::runtime_error("Unsupported layout.");
            }
        }

        for (std::size_t i = 0; i < childObjects.size(); ++i) {
            const auto& object{childObjects.at(i)};
            object->setAnchor(Anchor::topLeft);
            object->setPosition(positions.at(i));
            object->setLayer(layer());
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
} // namespace EconSimPlusPlus
