

#ifndef LIBTILEMAPEDITOR_TILEENGINE_EDITOR_MESSAGEDIALOG_HPP
#define LIBTILEMAPEDITOR_TILEENGINE_EDITOR_MESSAGEDIALOG_HPP

#include <functional>

#include "portable-file-dialogs.h"

#include <TileEngine/Editor/Dialog.hpp>

namespace TileEngine::Editor {

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

} // namespace TileEngine::Editor

#endif // LIBTILEMAPEDITOR_TILEENGINE_EDITOR_MESSAGEDIALOG_HPP
