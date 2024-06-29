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

#include <EconSimPlusPlus/Editor/OpenFileDialog.hpp>

namespace EconSimPlusPlus::Editor {
    OpenFileDialog::OpenFileDialog(const pfd::open_file& fileDialog, const std::function<void(std::string)>& callback) :
        m_fileDialog(fileDialog), m_callback(callback) {
        assert(not active() && "Cannot open a new dialog when one is already open.");
    }

    OpenFileDialog::~OpenFileDialog() {
        kill();
    }

    bool OpenFileDialog::active() const {
        return m_fileDialog.has_value();
    }

    void OpenFileDialog::update() {
        if (not active() or not m_fileDialog->ready(0)) {
            return;
        }

        if (const std::vector selection = m_fileDialog->result(); !selection.empty()) {
            m_callback(selection[0]);
        }

        m_fileDialog.reset();
    }

    void OpenFileDialog::kill() {
        if (not active()) {
            return;
        }

        m_fileDialog->kill();
        m_fileDialog.reset();
    }
} // namespace EconSimPlusPlus::Editor
