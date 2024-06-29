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
// Created by Anthony Dickson on 29/06/2024.
//

#ifndef LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_DIALOG_HPP
#define LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_DIALOG_HPP


namespace EconSimPlusPlus::Editor {

    /// Manages asynchronous dialog windows.
    class Dialog {
    public:
        virtual ~Dialog() = default;

        /// Check whether the dialog is currently open.
        /// @return `true` if the dialog is currently open, `false` otherwise.
        [[nodiscard]] virtual bool active() const = 0;

        /// Poll for the user's selection.
        virtual void update() = 0;

        /// Close the active file dialog, if one exists, without waiting for user input.
        virtual void kill() = 0;
    };

} // namespace EconSimPlusPlus::Editor

#endif // LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_DIALOG_HPP
