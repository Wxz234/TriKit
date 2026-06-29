#include "Renderer/Renderer.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <vector>

#define FRAME_BUFFER_COUNT 3

namespace TriKit {
    class RendererImpl : public Renderer {
    public:
        virtual ~RendererImpl() {
            Release();
        }

        void Present() override {
            swapChain->Present(1, 0);
        }

        bool Initialize(void* windowHandle, int width, int height) {
            // Initialization code for DirectX 12 renderer
            // Create device, command queues, swap chain, etc.
            hWnd = static_cast<HWND>(windowHandle);
            this->width = width;
            this->height = height;

            UINT32 factoryFlag = 0;
            Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
#ifdef _DEBUG
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
                debugController->EnableDebugLayer();
            }
            factoryFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif
            if (FAILED(CreateDXGIFactory2(factoryFlag, IID_PPV_ARGS(&factory)))) {
                Release();
                return false;
            }
            Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;
            if (FAILED(factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)))) {
                Release();
                return false;
            }
            if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)))) {
                Release();
                return false;
            }
            D3D12_COMMAND_QUEUE_DESC queueDesc{};
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&graphicsQueue)))) {
                Release();
                return false;
            }
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
            if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&copyQueue)))) {
                Release();
                return false;
            }
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&computeQueue)))) {
                Release();
                return false;
            }

            DXGI_SWAP_CHAIN_DESC1 scDesc{};
            scDesc.BufferCount = FRAME_BUFFER_COUNT;
            scDesc.Width = width;
            scDesc.Height = height;
            scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            scDesc.SampleDesc.Count = 1;
            scDesc.SampleDesc.Quality = 0;
            scDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
            scDesc.Scaling = DXGI_SCALING_STRETCH;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc{};
            fsSwapChainDesc.Windowed = TRUE;

            if (FAILED(factory->CreateSwapChainForHwnd(graphicsQueue.Get(), hWnd, &scDesc, &fsSwapChainDesc, nullptr, &swapChain1))) {
                Release();
                return false;
            }
            if (FAILED(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER))) {
                Release();
                return false;
            }
            if (FAILED(swapChain1.As(&swapChain))) {
                Release();
                return false;
            }
            fenceValue.resize(FRAME_BUFFER_COUNT, 0);
            frameIndex = swapChain->GetCurrentBackBufferIndex();
            if (FAILED(device->CreateFence(fenceValue[frameIndex]++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&graphicsFence)))) {
                Release();
                return false;
            }
            if (FAILED(device->CreateFence(copyFenceValue++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&copyFence)))) {
                Release();
                return false;
            }
            if (FAILED(device->CreateFence(computeFenceValue++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&computeFence)))) {
                Release();
                return false;
            }

            graphicsEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
            copyEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
            computeEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);

            factory.Reset();
            swapChain1.Reset();

            return true;
        }

        void Release() {
            if (graphicsEvent) {
                CloseHandle(graphicsEvent);
                graphicsEvent = nullptr;
            }
            if (copyEvent) {
                CloseHandle(copyEvent);
                copyEvent = nullptr;
            }
            if (computeEvent) {
                CloseHandle(computeEvent);
                computeEvent = nullptr;
            }
            computeFence.Reset();
            copyFence.Reset();
            graphicsFence.Reset();
            swapChain.Reset();
            swapChain1.Reset();
            computeQueue.Reset();
            copyQueue.Reset();
            graphicsQueue.Reset();
            device.Reset();
            factory.Reset();
        }

        void Shutdown() override {
            FlushQueue(graphicsQueue.Get(), graphicsFence.Get(), graphicsEvent, fenceValue[frameIndex]);
            FlushQueue(copyQueue.Get(), copyFence.Get(), copyEvent, copyFenceValue);
            FlushQueue(computeQueue.Get(), computeFence.Get(), computeEvent, computeFenceValue);
            Release();
        }

        void MoveToNextFrame() {
            const UINT64 currentFenceValue = fenceValue[frameIndex];
            graphicsQueue->Signal(graphicsFence.Get(), currentFenceValue);
            frameIndex = swapChain->GetCurrentBackBufferIndex();
            if (graphicsFence->GetCompletedValue() < fenceValue[frameIndex]) {
                graphicsFence->SetEventOnCompletion(fenceValue[frameIndex], graphicsEvent);
                WaitForSingleObjectEx(graphicsEvent, INFINITE, FALSE);
            }
            fenceValue[frameIndex] = currentFenceValue + 1;
        }

        void FlushQueue(ID3D12CommandQueue* queue, ID3D12Fence* fence, HANDLE event, UINT64 value) {
            queue->Signal(fence, value);
            fence->SetEventOnCompletion(value, event);
            WaitForSingleObjectEx(event, INFINITE, FALSE);
        }

        Microsoft::WRL::ComPtr<IDXGIFactory7> factory;
        Microsoft::WRL::ComPtr<ID3D12Device> device;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> graphicsQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> copyQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> computeQueue;
        Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
        Microsoft::WRL::ComPtr<ID3D12Fence> graphicsFence;
        Microsoft::WRL::ComPtr<ID3D12Fence> copyFence;
        Microsoft::WRL::ComPtr<ID3D12Fence> computeFence;

        UINT32 frameIndex = 0;
        std::vector<UINT64> fenceValue;
        UINT64 copyFenceValue = 0;
        UINT64 computeFenceValue = 0;

        HANDLE graphicsEvent = nullptr;
        HANDLE copyEvent = nullptr;
        HANDLE computeEvent = nullptr;

        int width = 0;
        int height = 0;
        HWND hWnd = nullptr;
    };

    Renderer* CreateRendererInstance() {
        return new RendererImpl;
    }

    void DestroyRendererInstance(Renderer* renderer) {
        delete renderer;
    }
}