

#include <EconSimPlusPlus/TwoColumnLayout.hpp>

namespace EconSimPlusPlus {
    TwoColumnLayout::TwoColumnLayout(const Layout layout) :
        m_layout(layout), m_groupLayout{.direction = Group::LayoutDirection::horizontal,
                                        .padding = glm::vec2{0.0f},
                                        .spacing = m_layout.spacing.x,
                                        .horizontalAlignment = Group::HorizontalAlignment::justified,
                                        .verticalAlignment = Group::VerticalAlignment::center} {
    }

    void TwoColumnLayout::addRow(const std::shared_ptr<Object>& left, const std::shared_ptr<Object>& right) {
        const auto group = std::make_shared<Group>(m_groupLayout);
        group->addChild(left);
        group->addChild(right);
        Object::addChild(group);
        recalculateLayout();
    }

    void TwoColumnLayout::setPosition(const glm::vec2 position) {
        const glm::vec2 delta{position - this->position()};
        Object::setPosition(position);

        for (const auto& object : children()) {
            object->setPosition(object->position() + delta);
        }
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

    void TwoColumnLayout::update(const float deltaTime, const InputState& inputState, const Camera& camera) {
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
        // ReSharper disable once CppTemplateArgumentsCanBeDeduced
        const std::vector<std::shared_ptr<Object>> childObjects{children()};

        // Calculate the row dimensions that can fully contain all rows.
        glm::vec2 maxRowDimensions{0.0f};

        for (const auto& group : childObjects) {
            const glm::vec2 rowSize = group->size();

            maxRowDimensions.x = std::max(maxRowDimensions.x, rowSize.x);
            maxRowDimensions.y = std::max(maxRowDimensions.y, rowSize.y);
        }

        // Update own size.
        const float rowCount{static_cast<float>(childObjects.size())};
        const glm::vec2 containingSize{maxRowDimensions * rowCount + m_layout.padding +
                                       glm::vec2{0.0f, m_layout.spacing.y * (rowCount - 1.0f)}};
        setSize(containingSize);

        // Update rows
        glm::vec2 nextPosition{topLeft(*this) + 0.5f * glm::vec2{m_layout.padding.x, -m_layout.padding.y}};

        for (auto& group : childObjects) {
            group->setAnchor(Anchor::topLeft);
            group->setSize(maxRowDimensions);
            group->setPosition(nextPosition);
            group->setLayer(layer());

            nextPosition = bottomLeft(*group) - glm::vec2{0.0f, m_layout.spacing.y};
        }
    }
} // namespace EconSimPlusPlus
