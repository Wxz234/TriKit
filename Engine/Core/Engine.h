#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

namespace TriKit {
    class Engine {
    private:
        static Engine* instance;

        Microsoft::WRL::ComPtr<ID3D12Device> device;
        Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
        Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;

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

        ID3D12Device* GetDevice() const;
        IDXGIFactory4* GetFactory() const;
        IDXGISwapChain* GetSwapChain() const;
    };
}