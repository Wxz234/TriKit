#pragma once

#include "Core/Event.h"

namespace TriKit {
    class Window {
    public:
        virtual bool Initialize(void* instance, int width, int height, const wchar_t* title) noexcept = 0;
        virtual bool PollEvents(Event& event) = 0;
        virtual void Shutdown() = 0;
        virtual void* GetNativeHandle() const = 0;
    private:
    };
}