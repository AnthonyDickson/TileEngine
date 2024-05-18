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
// Created by Anthony Dickson on 03/05/2024.
//

#ifndef LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_FRAMETIMER_HPP
#define LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_FRAMETIMER_HPP

#include <chrono>

namespace EconSimPlusPlus::Engine {
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

} // namespace EconSimPlusPlus::Engine

#endif // LIBECONSIMPLUSPLUS_ECONSIMPLUSPLUS_ENGINE_FRAMETIMER_HPP
