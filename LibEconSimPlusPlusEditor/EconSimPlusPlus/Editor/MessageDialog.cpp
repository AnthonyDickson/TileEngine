

#include <cassert>

#include <EconSimPlusPlus/Editor/MessageDialog.hpp>

namespace EconSimPlusPlus::Editor {
    MessageDialog::MessageDialog(const pfd::message& messageDialog, const std::function<void()>& yesCallback,
                                 const std::function<void()>& noCallback) :
        m_messageDialog(messageDialog), m_yesCallback(yesCallback), m_noCallback(noCallback) {
    }

    MessageDialog::~MessageDialog() {
        kill();
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
