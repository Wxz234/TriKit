#pragma once

#include <string>

namespace TriKit {
    struct WindowDesc {
        void* instance = nullptr;
        int         width = 1280;
        int         height = 720;
        std::wstring title = L"Engine";
    };

    class Window {
    public:
        bool Initialize(const WindowDesc& desc);
        void PollEvents();
        bool ShouldClose() const;
        void Shutdown();
    private:
        void* hInstance = nullptr;
        void* hWnd = nullptr;
        int width = 0;
        int height = 0;
        std::wstring title;
    };
}