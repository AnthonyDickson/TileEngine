

#include <TileEngine/Object.hpp>

#include "glm/ext/matrix_transform.hpp"

namespace TileEngine {
    Object::~Object() = default;

    bool Object::focusable() const {
        return m_focusable;
    }

    void Object::setFocusable(const bool value) {
        m_focusable = value;
    }

    glm::vec2 Object::position() const {
        return {m_position.x, m_position.y};
    }

    void Object::setPosition(const glm::vec2 position) {
        m_position.x = position.x;
        m_position.y = position.y;
    }

    float Object::layer() const {
        return m_position.z;
    }

    void Object::setLayer(const float layer) {
        assert(layer >= 0.0f && "Layer must be non-negative.");
        m_position.z = layer;
    }

    glm::vec2 Object::size() const {
        return m_size;
    }

    void Object::setSize(const glm::vec2 size) {
        assert(glm::all(glm::greaterThanEqual(size, glm::vec2{0.0f})) &&
               "All components of size must be greater than or equal to 0.0.");
        m_size = size;
    }

    Anchor Object::anchor() const {
        return m_anchor;
    }

    void Object::setAnchor(const Anchor anchor) {
        m_anchor = anchor;
    }

    const std::vector<std::shared_ptr<Object>>& Object::children() const {
        return m_children;
    }

    void Object::addChild(const std::shared_ptr<Object>& object) {
        m_children.push_back(object);
    }

    void Object::removeChild(const std::shared_ptr<Object>& object) {
        std::erase(m_children, object);
    }

    void Object::addEventHandler(const EventHandler& eventHandler) {
        m_eventHandlers.push_back(eventHandler);
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void Object::notify(const Event event, const EventData& eventData) {
        for (auto& handler : m_eventHandlers) {
            handler(event, eventData);
        }
    }

    std::vector<std::shared_ptr<Object>> traverse(const std::vector<std::shared_ptr<Object>>& objects) { // NOLINT(*-no-recursion)
        std::vector<std::shared_ptr<Object>> traversalOrder{};

        for (const auto& object : objects) {
            std::vector childTraversalOrder{traverse(object->children())};
            traversalOrder.insert(traversalOrder.end(), childTraversalOrder.begin(), childTraversalOrder.end());
            traversalOrder.push_back(object);
        }

        return traversalOrder;
    }

    bool contains(const Object& object, glm::vec2 point) {
        const glm::vec2 position{bottomLeft(object)};

        for (int axis = 0; axis < 2; ++axis) {
            const bool exceedsMinExtent = point[axis] < position[axis];
            // ReSharper disable once CppTooWideScopeInitStatement
            const bool exceedsMaxExtent = point[axis] > position[axis] + object.size()[axis];

            if (exceedsMinExtent or exceedsMaxExtent) {
                return false;
            }
        }

        return true;
    }

    float top(const Object& object) {
        const glm::vec2 anchorOffset{calculateAnchorOffset(object.size(), object.anchor())};

        return object.position().y + anchorOffset.y;
    }

    float bottom(const Object& object) {
        return top(object) - object.size().y;
    }

    float left(const Object& object) {
        const glm::vec2 anchorOffset{calculateAnchorOffset(object.size(), object.anchor())};

        return object.position().x + anchorOffset.x;
    }

    float right(const Object& object) {
        return left(object) + object.size().x;
    }

    glm::vec2 topLeft(const Object& object) {
        const glm::vec2 anchorOffset{calculateAnchorOffset(object.size(), object.anchor())};

        return object.position() + anchorOffset;
    }

    glm::vec2 topRight(const Object& object) {
        const glm::vec2 topLeft{TileEngine::topLeft(object)};

        return topLeft + glm::vec2{object.size().x, 0.0f};
    }

    glm::vec2 bottomLeft(const Object& object) {
        const glm::vec2 topLeft{TileEngine::topLeft(object)};

        return topLeft - glm::vec2{0.0f, object.size().y};
    }

    glm::vec2 bottomRight(const Object& object) {
        const glm::vec2 topLeft{TileEngine::topLeft(object)};

        return topLeft + glm::vec2{object.size().x, -object.size().y};
    }
} // namespace TileEngine
