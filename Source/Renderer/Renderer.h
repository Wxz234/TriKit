#pragma once

namespace TriKit {
    class Renderer {
    public:
        virtual bool Initialize(void* windowHandle, int width, int height) = 0;
        virtual void Present() = 0;
        virtual void Shutdown() = 0;
        virtual void MoveToNextFrame() = 0;
    };
}