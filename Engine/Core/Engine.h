#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <vector>
#include <string>

namespace TriKit {
    class Engine {
    private:
        // DirectX members
        Microsoft::WRL::ComPtr<ID3D12Device> device;
        Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
        Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

        Microsoft::WRL::ComPtr<ID3D12CommandQueue> graphicsQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> copyQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> computeQueue;

        Microsoft::WRL::ComPtr<ID3D12Fence> graphicsFence;
        Microsoft::WRL::ComPtr<ID3D12Fence> copyFence;
        Microsoft::WRL::ComPtr<ID3D12Fence> computeFence;

        HANDLE graphicsEvent = nullptr;
        HANDLE copyEvent = nullptr;
        HANDLE computeEvent = nullptr;

        // Window members
        HWND hWnd;
        HINSTANCE hInstance;
        int width;
        int height;
        std::wstring title;

        UINT32 frameIndex = 0;
        std::vector<UINT64> fenceValue;
        UINT64 copyFenceValue = 0;
        UINT64 computeFenceValue = 0;

        void Shutdown();
        void Present();
        void MoveToNextFrame();

        ID3D12CommandQueue* GetGraphicsQueue() const;
        ID3D12CommandQueue* GetCopyQueue() const;
        ID3D12CommandQueue* GetComputeQueue() const;

    public:
        Engine();
        ~Engine();

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) noexcept = delete;
        Engine& operator=(Engine&&) noexcept = delete;

        bool Initialize(HINSTANCE hInstance, int width, int height, const wchar_t* title);
        int Run();
    };
}