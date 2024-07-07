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

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_GROUP_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_GROUP_HPP

#include <EconSimPlusPlus/Object.hpp>

namespace EconSimPlusPlus {

    // TODO: Add option to draw outline/background. Create `Graphics` object that contains generic quad to use for
    // rendering?
    /// Automatically manages the layout of objects.
    class Group final : public Object {
    public:
        /// How objects in the group should be positioned relative to each other.
        enum class LayoutDirection { vertical, horizontal };

        /// Configuration for group layout.
        struct Layout {
            /// How objects in the group should be positioned relative to each other.
            LayoutDirection direction{LayoutDirection::vertical};
            /// The horizontal and vertical space between the inner borders of the group and its child objects in
            /// pixels.
            glm::vec2 padding{};
            /// The space between child objects in pixels.
            float spacing{};
        };

        /// Create an empty group.
        /// @param layout Configuration for group layout.
        explicit Group(Layout layout);

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

        /// Configuration for group layout.
        const Layout m_layout;
    };

} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_GROUP_HPP
