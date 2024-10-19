

#include <EconSimPlusPlus/Editor/SaveFileDialog.hpp>

namespace EconSimPlusPlus::Editor {
    SaveFileDialog::SaveFileDialog(const pfd::save_file& fileDialog, const std::function<void(std::string)>& callback) :
        m_fileDialog(fileDialog), m_callback(callback) {
    }

    SaveFileDialog::~SaveFileDialog() {
        kill();
    }

    bool SaveFileDialog::active() const {
        return m_fileDialog.has_value();
    }

    void SaveFileDialog::update() {
        if (not active() or not m_fileDialog->ready(0)) {
            return;
        }

        if (const std::string filePath = m_fileDialog->result(); not filePath.empty()) {
            m_callback(filePath);
        }

        m_fileDialog.reset();
    }

    void SaveFileDialog::kill() {
        if (not active()) {
            return;
        }

        m_fileDialog->kill();
        m_fileDialog.reset();
    }
} // namespace EconSimPlusPlus::Editor
