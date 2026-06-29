#include <Windows.h>

#include "Platform/Window.h"

#include <string>

namespace TriKit {
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { 
        switch (msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        return 0;
    }

    class WindowImpl : public Window {
    public:
        virtual ~WindowImpl() = default;

        virtual bool Initialize(void* instance, int width, int height, const wchar_t* title) noexcept override {
            hInstance = instance;
            this->width = width;
            this->height = height;
            this->title = title;

            const std::wstring trikitWindow = L"TriKitWndClass";

            WNDCLASSEXW wcex{};
            wcex.cbSize = sizeof(WNDCLASSEXW);
            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = WndProc;
            wcex.hInstance = static_cast<HINSTANCE>(this->hInstance);
            wcex.hIcon = LoadIconW(static_cast<HINSTANCE>(this->hInstance), L"IDI_ICON");
            wcex.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
            wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
            wcex.lpszClassName = trikitWindow.c_str();
            wcex.hIconSm = LoadIconW(static_cast<HINSTANCE>(this->hInstance), L"IDI_ICON");
            if (!RegisterClassExW(&wcex)) {
                return false;
            }
            auto stype = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
            RECT rc = { 0, 0, static_cast<LONG>(this->width), static_cast<LONG>(this->height) };
            AdjustWindowRect(&rc, stype, FALSE);
            hWnd = CreateWindowExW(0, trikitWindow.c_str(), this->title.c_str(), stype, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, static_cast<HINSTANCE>(this->hInstance), nullptr);
            if (!hWnd) {
                return false;
            }
            ShowWindow(static_cast<HWND>(hWnd), SW_SHOWDEFAULT);
            UpdateWindow(static_cast<HWND>(hWnd));

            return true;
        }

        virtual bool PollEvents(Event& event) override {
            if(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
                if (msg.message == WM_QUIT) {
                    event.type = EventType::WindowClose;
                    event.exitCode = static_cast<int>(msg.wParam);
                }

                return true;
            }
            return false;
        }

        virtual void Shutdown() override {}

        void* GetNativeHandle() const override { return hWnd; }

        void* hInstance = nullptr;
        void* hWnd = nullptr;
        int width = 0;
        int height = 0;
        std::wstring title;

        MSG msg = {};
    };

    Window* CreateWindowInstance() {
        return new WindowImpl;
    }

    void DestroyWindowInstance(Window* window) {
        delete window;
    }
}

