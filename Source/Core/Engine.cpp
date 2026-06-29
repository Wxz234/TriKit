#include <Windows.h>

#include "Core/Engine.h"

#include <entt/entt.hpp>

#include <string>

namespace TriKit {

    Window* CreateWindowInstance();
    void DestroyWindowInstance(Window* window);
    Renderer* CreateRendererInstance();
    void DestroyRendererInstance(Renderer* renderer);

    struct Engine::Impl {
        entt::registry registry;
        void* instance = nullptr;
        int width = 0;
        int height = 0;
        std::wstring title;

        LARGE_INTEGER frequency{};
        LARGE_INTEGER lastTime{};
    };

    Engine::Engine() noexcept {
        impl = new Impl;
        QueryPerformanceFrequency(&impl->frequency);
        QueryPerformanceCounter(&impl->lastTime);
    }

    Engine::~Engine() {
        delete impl;
    }

    void Engine::SetWindow(void* instance, int width, int height, const wchar_t* title) {
        impl->instance = instance;
        impl->width = width;
        impl->height = height;
        impl->title = title;
    }

    float Engine::CalculateDeltaTime() {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        float deltaTime = float(currentTime.QuadPart - impl->lastTime.QuadPart) / float(impl->frequency.QuadPart);
        impl->lastTime = currentTime;
        return deltaTime;
    }

    int Engine::Run() noexcept {
        window = CreateWindowInstance();
        renderer = CreateRendererInstance();

        if (!window->Initialize(impl->instance, impl->width, impl->height, impl->title.c_str())) {
            return 1;
        }
        if (!renderer->Initialize(window->GetNativeHandle(), impl->width, impl->height)) {
            return 1;
        }

        QueryPerformanceCounter(&impl->lastTime);

        bool running = true;
        Event event;
        while (running) {
            while (window->PollEvents(event)) {
                switch (event.type) {
                    case EventType::WindowClose:
                        running = false;
                        break;
                }
            }
            float deltaTime = CalculateDeltaTime();
            renderer->Present();
            renderer->MoveToNextFrame();
        }

        renderer->Shutdown();
        window->Shutdown();

        DestroyRendererInstance(renderer);
        DestroyWindowInstance(window);

        return event.exitCode;
    }
}
