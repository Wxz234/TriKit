#include "Engine.h"

namespace TriKit {
    Engine* Engine::instance = nullptr;

    Engine::Engine() {
    }

    Engine::~Engine() {
        Shutdown();
    }

    Engine* Engine::GetInstance() {
        if (!instance) {
            instance = new Engine();
        }
        return instance;
    }

    void Engine::DestroyInstance() {
        if (instance) {
            delete instance;
            instance = nullptr;
        }
    }

    bool Engine::Initialize(HINSTANCE hInstance, int width, int height, const wchar_t* title) {
        return false;
    }

    void Engine::Run() {
    }

    void Engine::Shutdown() {
    }

    void Engine::Present() {
    }

    void Engine::WaitForPreviousFrame() {
    }

    ID3D12Device* Engine::GetDevice() const {
        return device.Get();
    }

    IDXGIFactory4* Engine::GetFactory() const {
        return factory.Get();
    }

    IDXGISwapChain* Engine::GetSwapChain() const {
        return swapChain.Get();
    }

    HWND Engine::GetWindowHandle() const {
        return hWnd;
    }

    int Engine::GetWidth() const {
        return width;
    }

    int Engine::GetHeight() const {
        return height;
    }

}
