#include "Core/Engine.h"

using namespace TriKit;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    Engine& engine = Engine::GetInstance();
    if (!engine.Initialize(hInstance, 1280, 720, L"TriKit Engine - Basic Example")) {
        return 1;
    }
    return engine.Run();
}