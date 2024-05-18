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

#include <EconSimPlusPlus/Engine/FrameTimer.hpp>

namespace EconSimPlusPlus::Engine {
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
} // namespace EconSimPlusPlus::Engine
