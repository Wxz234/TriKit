#pragma once

namespace TriKit {
    enum class EventType {
        None = 0,
        WindowClose,
    };

    struct Event {
        EventType type = EventType::None;
        int exitCode = 0;
    };
}