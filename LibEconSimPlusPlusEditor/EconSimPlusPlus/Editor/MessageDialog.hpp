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

#ifndef LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_MESSAGEDIALOG_HPP
#define LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_MESSAGEDIALOG_HPP

#include <functional>

#include "portable-file-dialogs.h"

#include <EconSimPlusPlus/Editor/Dialog.hpp>

namespace EconSimPlusPlus::Editor {

    /// Manages a message dialog (yes, no, cancel) asynchronously.
    class MessageDialog final : public Dialog {
    public:
        /// Open a message dialog and register a callback.
        /// @param messageDialog An active message dialog that has been opened asynchronously.
        /// @param yesCallback The function to call when the user clicks the yes button.
        /// @param noCallback The function to call when the user clicks the no button.
        MessageDialog(const pfd::message& messageDialog, const std::function<void()>& yesCallback,
                  const std::function<void()>& noCallback);

        ~MessageDialog() override;

        /// Check whether a message dialog is currently open.
        /// @return `true` if a message dialog is currently open, `false` otherwise.
        [[nodiscard]] bool active() const override;

        /// Poll for the user's selection.
        void update() override;

        /// Close the active message dialog, if one exists, without waiting for user input.
        void kill() override;

    private:
        /// The active message dialog, if one exists.
        std::optional<pfd::message> m_messageDialog;
        std::function<void()> m_yesCallback;
        std::function<void()> m_noCallback;
    };

} // namespace EconSimPlusPlus::Editor

#endif // LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_MESSAGEDIALOG_HPP
