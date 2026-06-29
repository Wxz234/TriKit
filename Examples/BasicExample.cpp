#include <Windows.h>

#include <Core/Engine.h>

using namespace TriKit;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    Engine* engine = Engine::GetInstance();
    engine->SetWindow(hInstance, 1280, 720, L"TriKit Engine - Basic Example");
    return engine->Run();
}