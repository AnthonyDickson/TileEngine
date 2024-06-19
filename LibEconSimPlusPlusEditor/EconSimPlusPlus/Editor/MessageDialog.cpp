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
// Created by Anthony Dickson on 19/06/2024.
//

#include <cassert>

#include <EconSimPlusPlus/Editor/MessageDialog.hpp>

namespace EconSimPlusPlus::Editor {


    void MessageDialog::open(const pfd::message& messageDialog, const std::function<void()>& yesCallback,
                             const std::function<void()>& noCallback) {
        assert(not active() && "Cannot open a new dialog when one is already open.");
        m_messageDialog = messageDialog;
        m_yesCallback = yesCallback;
        m_noCallback = noCallback;
    }

    bool MessageDialog::active() const {
        return m_messageDialog.has_value();
    }

    void MessageDialog::update() {
        if (not active() or not m_messageDialog->ready(0)) {
            return;
        }

        switch (m_messageDialog->result()) {
        case pfd::button::yes:
            m_yesCallback();
            break;
        case pfd::button::no:
            m_noCallback();
            break;
        case pfd::button::cancel:
            break;
        default:
            throw std::runtime_error("Unexpected result from message dialog.");
        }

        m_messageDialog.reset();
    }

    void MessageDialog::kill() {
        if (not active()) {
            return;
        }

        m_messageDialog->kill();
        m_messageDialog.reset();
    }

} // namespace EconSimPlusPlus::Editor
