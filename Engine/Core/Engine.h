#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace TriKit {
    class Engine {
    private:
        static Engine* instance;

        Engine();
        ~Engine();
    public:
        static Engine* GetInstance();
        static void DestroyInstance();

        bool Initialize(HINSTANCE hInstance, int width, int height, const wchar_t* title);
        void Run();
        void Shutdown();
        void Present();
        void WaitForPreviousFrame();
    };
}