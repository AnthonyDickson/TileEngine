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

#ifndef TWOCOLUMNLAYOUT_HPP
#define TWOCOLUMNLAYOUT_HPP

#include <EconSimPlusPlus/Group.hpp>
#include <EconSimPlusPlus/Object.hpp>

namespace EconSimPlusPlus {
    /// A group that organizes content into two columns.
    /// A two column layout has the following properties:
    /// - Organizes child objects into rows.
    /// - When calculating layout, the container queries the minimal size of each row.
    /// - A row's width is then set to the widest row in the grid.
    /// - Items in rows are centered vertically.
    /// - Items in rows are aligned to their closest edge.
    /// @note You should use the method `.addRow(...)` to add a pair of objects belonging to the same row. The method
    /// `.addChild(...)` will create a row with only one object.
    class TwoColumnLayout final : public Object {
    public:
        /// Configuration for group layout.
        struct Layout {
            /// The horizontal and vertical space between the inner borders of the group and its child objects in
            /// pixels.
            glm::vec2 padding{};
            /// The horizontal space between objects in a row (x) and the vertical space between rows (y), respectively
            /// (measured in pixels).
            glm::vec2 spacing{};
        };

        /// Create an empty two column layout.
        /// @param layout Configuration for two columnn layout.
        explicit TwoColumnLayout(Layout layout);

        /// Create a new row with a pair of objects and add it to the bottom of the group.
        /// @param left The object to appear in the left column of the row.
        /// @param right The object to appear in the right column of the row.
        void addRow(const std::shared_ptr<Object>& left, const std::shared_ptr<Object>& right);

        void setPosition(glm::vec2 position) override;
        void setLayer(float layer) override;
        void addChild(const std::shared_ptr<Object>& object) override;

        void update(float deltaTime, const InputState& inputState, const Camera& camera) override;
        void render(const Graphics& graphics) const override;

    private:
        /// Recalculate the group layout from scratch.
        /// @note Intended to be called after the group's position is updated or an object is added so that objects are
        /// positioned correctly.
        void recalculateLayout();

        /// Configuration for two column layout.
        Layout m_layout;
        /// Configuration for the row (group) layout.
        Group::Layout m_groupLayout;
    };

} // namespace EconSimPlusPlus

#endif // TWOCOLUMNLAYOUT_HPP
