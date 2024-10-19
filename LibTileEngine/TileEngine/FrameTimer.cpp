

#include <TileEngine/FrameTimer.hpp>

namespace TileEngine {
    FrameTimer::FrameTimer(const float alpha) : m_alpha(alpha) {
    }

    void FrameTimer::startStep() {
        m_stepStartTime = std::chrono::steady_clock::now();
    }

    void FrameTimer::endStep() {
        const std::chrono::duration stepElapsed{std::chrono::steady_clock::now() - m_stepStartTime};
        m_averageStepDuration = m_alpha * stepElapsed + (1.0f - m_alpha) * m_averageStepDuration;
    }

    float FrameTimer::average() const {
        return m_averageStepDuration.count() / 1e6f;
    }
} // namespace TileEngine
