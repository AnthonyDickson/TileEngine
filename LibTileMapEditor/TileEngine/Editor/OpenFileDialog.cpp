

#include <cassert>

#include <TileEngine/Editor/OpenFileDialog.hpp>

namespace TileEngine::Editor {
    OpenFileDialog::OpenFileDialog(const pfd::open_file& fileDialog, const std::function<void(std::string)>& callback) :
        m_fileDialog(fileDialog), m_callback(callback) {
    }

    OpenFileDialog::~OpenFileDialog() {
        kill();
    }

    bool OpenFileDialog::active() const {
        return m_fileDialog.has_value();
    }

    void OpenFileDialog::update() {
        if (not active() or not m_fileDialog->ready(0)) {
            return;
        }

        if (const std::vector selection = m_fileDialog->result(); !selection.empty()) {
            m_callback(selection[0]);
        }

        m_fileDialog.reset();
    }

    void OpenFileDialog::kill() {
        if (not active()) {
            return;
        }

        m_fileDialog->kill();
        m_fileDialog.reset();
    }
} // namespace TileEngine::Editor
