#pragma once

#include "EngineTypes.h"

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace TriKit {
    class Engine {
    private:
        using TickCallback = std::function<void(float)>;

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

        bool isInitialize = false;
        bool isDebugMode = false;

        LARGE_INTEGER frequency;
        LARGE_INTEGER lastTime;

        std::unordered_map<UINT64, TickCallback> tickCallbacks[4];
        std::unordered_map<UINT64, UpdateStage> idToStage;
        UINT64 callbackCount = 0;

        void Present();
        void MoveToNextFrame();

        float CalculateDeltaTime();

        ID3D12CommandQueue* GetGraphicsQueue() const;
        ID3D12CommandQueue* GetCopyQueue() const;
        ID3D12CommandQueue* GetComputeQueue() const;

        void FlushQueue(ID3D12CommandQueue* queue, ID3D12Fence* fence, HANDLE event, UINT64 value);

        Engine();
        ~Engine();

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) noexcept = delete;
        Engine& operator=(Engine&&) noexcept = delete;

        struct Impl;
        Impl* impl = nullptr;

    public:
        static Engine& GetInstance() {
            static Engine instance;
            return instance;
        }

        UINT64 AddTickCallback(TickCallback fn, UpdateStage stage = UpdateStage::Update);
        void RemoveTickCallback(UINT64 id);
        void EnableDebug(bool e) { isDebugMode = e; }
        bool Initialize(HINSTANCE hInstance, int width, int height, const wchar_t* title);
        int Run();
    };
}