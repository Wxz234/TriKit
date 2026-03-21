#include "Engine.h"

#define FRAME_BUFFER_COUNT 3

namespace TriKit {

    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { 
        if (msg == WM_DESTROY) { 
            PostQuitMessage(0); return 0; 
        } 
        return DefWindowProcW(hwnd, msg, wParam, lParam); 
    }

    Engine::Engine() {
        this->hInstance = nullptr;
        this->hWnd = nullptr;
        this->width = 0;
        this->height = 0;
    }

    Engine::~Engine() {
        Shutdown();
    }

    bool Engine::Initialize(HINSTANCE hInstance, int width, int height, const wchar_t* title) {

        this->hInstance = hInstance;
        this->width = width;
        this->height = height;
        this->title = title;

        WNDCLASSEXW wcex{};
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.hInstance = this->hInstance;
        wcex.hIcon = LoadIconW(this->hInstance, L"IDI_ICON");
        wcex.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
        wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcex.lpszClassName = L"TriKit.Window";
        wcex.hIconSm = LoadIconW(this->hInstance, L"IDI_ICON");
        
        if (!RegisterClassExW(&wcex)) {
            return false;
        }

        auto stype = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;
        RECT rc = { 0, 0, static_cast<LONG>(this->width), static_cast<LONG>(this->height) };
        AdjustWindowRect(&rc, stype, FALSE);
        hWnd = CreateWindowExW(0, L"TriKit.Window", this->title.c_str(), stype, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, this->hInstance, nullptr);
        
        if (!hWnd) {
            return false;
        }
        
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        Microsoft::WRL::ComPtr<IDXGIFactory7> factory;
        UINT32 factoryFlag = 0;
        Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
#ifdef _DEBUG
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
        }
        factoryFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif
        
        if (FAILED(CreateDXGIFactory2(factoryFlag, IID_PPV_ARGS(&factory)))) {
            return false;
        }

        Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;
        if (FAILED(factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)))) {
            return false;
        }

        if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)))) {
            return false;
        }

        D3D12_COMMAND_QUEUE_DESC queueDesc{};
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&graphicsQueue)))) {
            return false;
        }

        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
        if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&copyQueue)))) {
            return false;
        }

        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
        if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&computeQueue)))) {
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
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;

        if (FAILED(factory->CreateSwapChainForHwnd(graphicsQueue.Get(), hWnd, &scDesc, &fsSwapChainDesc, nullptr, &swapChain1))) {
            return false;
        }

        if (FAILED(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER))) {
            return false;
        }

        if (FAILED(swapChain1.As(&swapChain))) {
            return false;
        }

        fenceValue.resize(FRAME_BUFFER_COUNT, 0);
        frameIndex = swapChain->GetCurrentBackBufferIndex();
        if (FAILED(device->CreateFence(fenceValue[frameIndex]++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&graphicsFence)))) {
            return false;
        }

        if (FAILED(device->CreateFence(copyFenceValue++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&copyFence)))) {
            return false;
        }

        if (FAILED(device->CreateFence(computeFenceValue++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&computeFence)))) {
            return false;
        }

        graphicsEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        copyEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);
        computeEvent = CreateEventW(nullptr, FALSE, FALSE, nullptr);

        return true;
    }

    int Engine::Run() {
        MSG msg = {};
        while (msg.message != WM_QUIT) {
            if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg); 
                DispatchMessageW(&msg); 
            } else {
                
                Present();
                MoveToNextFrame();
            }
        }
        return static_cast<int>(msg.wParam);
    }

    void Engine::Shutdown() {
        graphicsQueue->Signal(graphicsFence.Get(), fenceValue[frameIndex]);
        graphicsFence->SetEventOnCompletion(fenceValue[frameIndex], graphicsEvent);
        WaitForSingleObjectEx(graphicsEvent, INFINITE, FALSE);
        fenceValue[frameIndex]++;

        copyQueue->Signal(copyFence.Get(), copyFenceValue);
        copyFence->SetEventOnCompletion(copyFenceValue, copyEvent);
        WaitForSingleObjectEx(copyEvent, INFINITE, FALSE);
        copyFenceValue++;

        computeQueue->Signal(computeFence.Get(), computeFenceValue);
        computeFence->SetEventOnCompletion(computeFenceValue, computeEvent);
        WaitForSingleObjectEx(computeEvent, INFINITE, FALSE);
        computeFenceValue++;
    }

    void Engine::Present() {
        swapChain->Present(1, 0);
    }

    void Engine::MoveToNextFrame() {
        const UINT64 currentFenceValue = fenceValue[frameIndex];
        graphicsQueue->Signal(graphicsFence.Get(), currentFenceValue);
        frameIndex = swapChain->GetCurrentBackBufferIndex();
        if (graphicsFence->GetCompletedValue() < fenceValue[frameIndex]) {
            graphicsFence->SetEventOnCompletion(fenceValue[frameIndex], graphicsEvent);
            WaitForSingleObjectEx(graphicsEvent, INFINITE, FALSE);
        }
        fenceValue[frameIndex] = currentFenceValue + 1;
    }

    ID3D12CommandQueue* Engine::GetGraphicsQueue() const {
        return graphicsQueue.Get();
    }

    ID3D12CommandQueue* Engine::GetCopyQueue() const {
        return copyQueue.Get();
    }

    ID3D12CommandQueue* Engine::GetComputeQueue() const {
        return computeQueue.Get();
    }

}
