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

}
