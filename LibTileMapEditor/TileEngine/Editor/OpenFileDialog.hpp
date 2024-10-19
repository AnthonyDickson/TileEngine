

#ifndef LIBTILEMAPEDITOR_TILEENGINE_EDITOR_OPENFILEDIALOG_HPP
#define LIBTILEMAPEDITOR_TILEENGINE_EDITOR_OPENFILEDIALOG_HPP

#include <functional>

#include "portable-file-dialogs.h"

#include <TileEngine/Editor/Dialog.hpp>

namespace TileEngine::Editor {

    /// Manages an open file dialog asynchronously.
    class OpenFileDialog final : public Dialog {
    public:
        /// Open a file dialog and register a callback.
        /// @param fileDialog An active file dialog that has been opened asynchronously.
        /// @param callback The function to be called if the user chooses a file.
        OpenFileDialog(const pfd::open_file& fileDialog, const std::function<void(std::string)>& callback);

        ~OpenFileDialog() override;

        /// Check whether a file dialog is currently open.
        /// @return `true` if a file dialog is currently open, `false` otherwise.
        [[nodiscard]] bool active() const override;

        /// Poll for the user's selection.
        void update() override;

        /// Close the active file dialog, if one exists, without waiting for user input.
        void kill() override;

    private:
        /// The active file dialog, if one exists.
        std::optional<pfd::open_file> m_fileDialog;
        /// The callback function to be called when the user selects a file.
        std::function<void(std::string)> m_callback;
    };

} // namespace TileEngine::Editor

#endif // LIBTILEMAPEDITOR_TILEENGINE_EDITOR_OPENFILEDIALOG_HPP
