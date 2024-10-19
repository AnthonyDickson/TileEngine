#include <iostream>

#include "Editor.hpp"

int main() {
    try {
        auto editor{EconSimPlusPlus::Editor::Editor::create({1920, 1080})};
        editor.run();
    }
    catch (const std::exception& exception) {
        std::cout << "Program exited with unhandled exception: " << exception.what() << std::endl;
    }

    return 0;
}
