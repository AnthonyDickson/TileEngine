

#include <stdexcept>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <TileEngine/KeyModifier.hpp>

namespace TileEngine::KeyModifier {
    KeyModifier operator|(const KeyModifier a, const KeyModifier b) {
        return static_cast<KeyModifier>(static_cast<int>(a) | static_cast<int>(b));
    }

    KeyModifier operator|=(KeyModifier& a, const KeyModifier b) {
        a = a | b;
        return a;
    }

    KeyModifier operator&(const KeyModifier a, const KeyModifier b) {
        return static_cast<KeyModifier>(static_cast<int>(a) & static_cast<int>(b));
    }

    bool contains(const KeyModifier flags, const KeyModifier target) {
        return (flags & target) == target;
    }

    bool contains(const KeyModifier flags, const int target) {
        switch (target) {
        case GLFW_MOD_CONTROL:
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
            return contains(flags, control);
        case GLFW_MOD_SHIFT:
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            return contains(flags, shift);
        case GLFW_MOD_ALT:
        case GLFW_KEY_LEFT_ALT:
        case GLFW_KEY_RIGHT_ALT:
            return contains(flags, alt);
        case GLFW_MOD_SUPER:
        case GLFW_KEY_LEFT_SUPER:
        case GLFW_KEY_RIGHT_SUPER:
            return contains(flags, super);
        default:
            throw std::invalid_argument("The given key code is not a key modifier (control, shift, alt or super).");
        }
    }
} // namespace TileEngine::KeyModifier
