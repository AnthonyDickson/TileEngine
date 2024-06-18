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
// Created by Anthony Dickson on 02/06/2024.
//

#include <cassert>

#include <EconSimPlusPlus/Editor/SaveFileDialog.hpp>

namespace EconSimPlusPlus::Editor {
    void SaveFileDialog::open(const pfd::save_file& fileDialog, const std::function<void(std::string)>& callback) {
        assert(not active() && "Cannot open a new dialog when one is already open.");
        m_fileDialog = fileDialog;
        m_callback = callback;
    }

    bool SaveFileDialog::active() const {
        return m_fileDialog.has_value();
    }

    void SaveFileDialog::update() {
        if (not active() or not m_fileDialog->ready(0)) {
            return;
        }

        // TODO: Find way to check which button was clicked (e.g., the cancel button).
        if (const std::string filePath = m_fileDialog->result(); not filePath.empty()) {
            m_callback(filePath);
        }

        m_fileDialog.reset();
    }

    void SaveFileDialog::kill() {
        if (not active()) {
            return;
        }

        m_fileDialog->kill();
        m_fileDialog.reset();
    }
} // namespace EconSimPlusPlus::Editor
