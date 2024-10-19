

#ifndef LIBTILEENGINE_TILEENGINE_FRAMETIMER_HPP
#define LIBTILEENGINE_TILEENGINE_FRAMETIMER_HPP

#include <chrono>

namespace TileEngine {
    /// Measures the average time for repeating tasks such as rendering frames.
    class FrameTimer {
    public:
        /// Create a frame timer.
        /// @param alpha The interpolation factor between steps.
        explicit FrameTimer(float alpha = 0.01f);

        /// Start measuring the duration of a step.
        void startStep();

        /// End measuring the duration of a step.
        void endStep();

        /// Get the average step duration.
        /// @return the duration in milliseconds.
        [[nodiscard]] float average() const;

    private:
        /// The interpolation factor between steps.
        /// @note Alpha is the amount of the new reading to keep and 1 - alpha is the amount of the accumulated readings
        /// to keep.
        const float m_alpha;
        /// The exponential moving average time per step measured in nanoseconds.
        std::chrono::duration<float, std::ratio<1, 1000000000>> m_averageStepDuration{};
        /// The time when the current step was started.
        std::chrono::time_point<std::chrono::steady_clock> m_stepStartTime{};
    };

} // namespace TileEngine

#endif // LIBTILEENGINE_TILEENGINE_FRAMETIMER_HPP
