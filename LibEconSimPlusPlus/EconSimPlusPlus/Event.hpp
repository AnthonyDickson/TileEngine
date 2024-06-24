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
// Created by Anthony Dickson on 11/06/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_EVENT_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_EVENT_HPP

#include <functional>

#include "glm/vec2.hpp"

#include <EconSimPlusPlus/Window.hpp>

namespace EconSimPlusPlus {
    /// Global event types.
    enum class Event { tileMapLoaded, windowResize, mouseEnter, mouseLeave, mouseHover, mouseClick };

    /// The data to accompany an event notification.
    struct EventData {
        /// The window the event happened in.
        Window& window;
        /// Where the event happened (only applicaple to events that involve mouse input).
        std::optional<glm::vec2> mousePosition;
    };

    using EventHandler = std::function<void(Event event, EventData eventData)>;
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_EVENT_HPP
