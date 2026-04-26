#include "Engine/Core/Engine.h"
#include "Engine/Core/GameState.h"

using namespace TriKit;

void Update(GameState& state, float deltaTime) {

}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    Engine& engine = Engine::GetInstance();
    if (!engine.Initialize(hInstance, 1280, 720, L"TriKit Engine - Basic Example")) {
        return 1;
    }
    engine.AddUpdateCallback(Update);
    return engine.Run();
}