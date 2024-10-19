

#ifndef LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_SAVEFILEDIALOG_HPP
#define LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_SAVEFILEDIALOG_HPP

#include <functional>

#include "portable-file-dialogs.h"

#include <EconSimPlusPlus/Editor/Dialog.hpp>

namespace EconSimPlusPlus::Editor {

    /// Manages an save file dialog asynchronously.
    class SaveFileDialog final : public Dialog {
    public:
        /// Open a file dialog and register a callback.
        /// @param fileDialog An active file dialog that has been opened asynchronously.
        /// @param callback The function to be called if the user chooses a file.
        SaveFileDialog(const pfd::save_file& fileDialog, const std::function<void(std::string)>& callback);

        ~SaveFileDialog() override;

        /// Check whether a file dialog is currently open.
        /// @return `true` if a file dialog is currently open, `false` otherwise.
        [[nodiscard]] bool active() const override;

        /// Poll for the user's selection.
        void update() override;

        /// Close the active file dialog, if one exists, without waiting for user input.
        void kill() override;

    private:
        /// The active file dialog, if one exists.
        std::optional<pfd::save_file> m_fileDialog;
        /// The callback function to be called when the user selects a file.
        std::function<void(std::string)> m_callback;
    };

} // namespace EconSimPlusPlus::Editor

#endif // LIBECONSIMPLUSPLUSEDITOR_ECONSIMPLUSPLUS_EDITOR_SAVEFILEDIALOG_HPP
