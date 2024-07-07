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
//

#include "glm/ext/matrix_transform.hpp"

#include <EconSimPlusPlus/Group.hpp>

namespace EconSimPlusPlus {
    namespace {
        /// Calculate the size of the group that would contain all objects plus padding and spacing.
        /// @param objects A list of objects that the group contains.
        /// @param style The group style containing the padding and spacing parameters.
        /// @return The width and height in pixels.
        glm::vec2 calculateSize(const std::vector<std::shared_ptr<Object>>& objects, const Group::Layout style) {
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
    } // namespace

    Group::Group(const Layout layout, const Style style) : m_layout(layout), m_style(style) {
    }

    void Group::setPosition(const glm::vec2 position) {
        Object::setPosition(position);
        recalculateLayout();
    }

    void Group::setLayer(const float layer) {
        Object::setLayer(layer);

        for (const auto& object : children()) {
            object->setLayer(layer);
        }
    }

    void Group::addChild(const std::shared_ptr<Object>& object) {
        Object::addChild(object);
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
            Outline::draw(*this, graphics.quadShader, graphics.quad, *m_style.outline);
        }

        for (const std::shared_ptr<Object>& object : children()) {
            object->render(graphics);
        }
    }

    void Group::recalculateLayout() {
        setSize(calculateSize(children(), m_layout));

        glm::vec2 nextPosition{topLeft(*this) + 0.5f * glm::vec2{m_layout.padding.x, -m_layout.padding.y}};

        for (auto& object : children()) {
            object->setAnchor(Anchor::topLeft);
            object->setPosition(nextPosition);
            object->setLayer(layer());

            switch (m_layout.direction) {
            case LayoutDirection::vertical:
                nextPosition = bottomLeft(*object) + glm::vec2{0.0f, -m_layout.spacing};
                break;
            case LayoutDirection::horizontal:
                nextPosition = topRight(*object) + glm::vec2(m_layout.spacing, 0.0f);
                break;
            default:
                throw std::runtime_error("Unsupported layout.");
            }
        }
    }
} // namespace EconSimPlusPlus
