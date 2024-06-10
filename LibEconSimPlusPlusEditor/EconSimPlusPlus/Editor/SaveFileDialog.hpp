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

#ifndef LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_SAVEFILEDIALOG_HPP
#define LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_SAVEFILEDIALOG_HPP

#include <functional>

#include "portable-file-dialogs.h"

namespace EconSimPlusPlus::Editor {

    /// Manages an save file dialog asynchronously.
    class SaveFileDialog {
    public:
        /// Open a file dialog and register a callback.
        /// @param fileDialog An active file dialog that has been opened asynchronously.
        /// @param callback The function to be called if the user chooses a file.
        void open(const pfd::save_file& fileDialog, const std::function<void(std::string)>& callback);

        /// Check whether a file dialog is currently open.
        /// @return `true` if a file dialog is currently open, `false` otherwise.
        [[nodiscard]] bool active() const;

        /// Poll for the user's selection.
        void update();

        /// Close the active file dialog, if one exists, without waiting for user input.
        void kill();

    private:
        /// The active file dialog, if one exists.
        std::optional<pfd::save_file> m_fileDialog{};
        /// The callback function to be called when the user selects a file.
        std::function<void(std::string)> m_callback{};
    };

} // namespace EconSimPlusPlus::Editor

#endif // LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_SAVEFILEDIALOG_HPP
