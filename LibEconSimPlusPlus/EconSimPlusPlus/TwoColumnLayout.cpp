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
// Created by Anthony Dickson on 09/07/2024.
//

#include <EconSimPlusPlus/TwoColumnLayout.hpp>

namespace EconSimPlusPlus {
    TwoColumnLayout::TwoColumnLayout(const Layout layout) :
        m_layout(layout), m_groupLayout{.direction = Group::LayoutDirection::horizontal,
                                        .padding = glm::vec2{0.0f},
                                        .spacing = m_layout.spacing.x} {
    }

    void TwoColumnLayout::addRow(const std::shared_ptr<Object>& left, const std::shared_ptr<Object>& right) {
        const auto group = std::make_shared<Group>(m_groupLayout);
        group->addChild(left);
        group->addChild(right);

        addChild(group);
    }

    void TwoColumnLayout::setPosition(const glm::vec2 position) {
        Object::setPosition(position);
        recalculateLayout();
    }

    void TwoColumnLayout::setLayer(const float layer) {
        Object::setLayer(layer);

        for (const auto& object : children()) {
            object->setLayer(layer);
        }
    }

    void TwoColumnLayout::addChild(const std::shared_ptr<Object>& object) {
        const auto group = std::make_shared<Group>(m_groupLayout);
        group->addChild(object);

        Object::addChild(group);
        recalculateLayout();
    }

    void TwoColumnLayout::update(float deltaTime, const InputState& inputState, const Camera& camera) {
        for (const std::shared_ptr<Object>& object : children()) {
            object->update(deltaTime, inputState, camera);
        }
    }

    void TwoColumnLayout::render(const Graphics& graphics) const {
        for (const std::shared_ptr<Object>& object : children()) {
            object->render(graphics);
        }
    }

    void TwoColumnLayout::recalculateLayout() {
        auto childObjects = children();

        // Calculate the row dimensions that can fully contain all rows.
        glm::vec2 maxRowDimensions{0.0f};

        for (const auto& group : childObjects) {
            const glm::vec2 rowSize = calculateSize(group->children(), m_groupLayout);

            if (rowSize.x > maxRowDimensions.x) {
                maxRowDimensions.x = rowSize.x;
            }

            if (rowSize.y > maxRowDimensions.y) {
                maxRowDimensions.y = rowSize.y;
            }
        }

        // Update rows
        glm::vec2 nextPosition{topLeft(*this) + 0.5f * glm::vec2{m_layout.padding.x, -m_layout.padding.y}};

        for (auto& group : childObjects) {
            group->setAnchor(Anchor::topLeft);
            group->setPosition(nextPosition);
            group->setSize(maxRowDimensions);
            group->setLayer(layer());

            nextPosition = bottomLeft(*group) + glm::vec2{0.0f, -m_layout.spacing.y};
        }

        // Update own size.
        const float rowCount{static_cast<float>(childObjects.size())};
        const glm::vec2 containingSize{maxRowDimensions * rowCount + m_layout.padding +
                                       glm::vec2{0.0f, m_layout.spacing.y * (rowCount - 1.0f)}};
        setSize(containingSize);
    }
} // namespace EconSimPlusPlus
