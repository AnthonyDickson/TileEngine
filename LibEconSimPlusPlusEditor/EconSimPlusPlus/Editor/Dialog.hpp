

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
