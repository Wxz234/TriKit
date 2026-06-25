#include "Window.h"
namespace TriKit {
    bool Window::Initialize(const WindowDesc& desc) {
        return false;
    }

    void Window::PollEvents() {

    }
    bool Window::ShouldClose() const {
        return false;
    }

    void Window::Shutdown() {
    }
}

