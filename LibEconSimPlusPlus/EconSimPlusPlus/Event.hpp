

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_EVENT_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_EVENT_HPP

#include <functional>
#include <optional>

#include "glm/vec2.hpp"

#include <EconSimPlusPlus/KeyModifier.hpp>
#include <EconSimPlusPlus/Window.hpp>

namespace EconSimPlusPlus {
    /// Global event types.
    enum class Event {
        tileMapLoaded,
        windowResize,
        mouseEnter,
        mouseLeave,
        mouseHover,
        mouseClick,
        focus,
        defocus,
        keyDown
    };

    /// The data to accompany an event notification.
    struct EventData {
        /// The window the event happened in.
        Window& window;
        /// Where the event happened (only applicable to events that involve mouse input).
        std::optional<glm::vec2> mousePosition{};
        /// Which key was pressed (only applicable to events that involve keyboard input).
        std::optional<int> keyCode{};
        /// Which key modifiers were pressed (only applicable to events that involve keyboard input).
        std::optional<KeyModifier::KeyModifier> keyModifiers{};
    };

    using EventHandler = std::function<void(Event event, EventData eventData)>;
} // namespace EconSimPlusPlus

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_EVENT_HPP
