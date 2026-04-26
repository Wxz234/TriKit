add_library(TriKitCore STATIC
    "Core/Engine.cpp"
    "Core/Engine.h"
    "Core/GameState.h"
)

target_include_directories(TriKitCore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_link_libraries(TriKitCore PUBLIC
    "d3d12.lib"
    "dxgi.lib"
)