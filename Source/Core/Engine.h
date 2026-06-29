#pragma once

#include "Platform/Window.h"
#include "Renderer/Renderer.h"

namespace TriKit {
    class Engine {
    public:
        static Engine* GetInstance() {
            static Engine instance;
            return &instance;
        }

        void SetWindow(void* instance, int width, int height, const wchar_t* title);
        int Run() noexcept;
    private:
        float CalculateDeltaTime();

        Engine() noexcept;
        ~Engine();
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine(Engine&&) noexcept = delete;
        Engine& operator=(Engine&&) noexcept = delete;

        struct Impl;
        Impl* impl = nullptr;

        Window* window = nullptr;
        Renderer* renderer = nullptr;
    };
}