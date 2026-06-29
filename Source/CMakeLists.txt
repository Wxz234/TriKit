file(GLOB ALL_SOURCES CONFIGURE_DEPENDS 
    "Core/*.cpp" "Core/*.h"
    "Platform/*.cpp" "Platform/*.h"
    "Renderer/*.cpp" "Renderer/*.h"
)

add_library(TriKitCore STATIC
    ${ALL_SOURCES}
)

target_include_directories(TriKitCore PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(TriKitCore PUBLIC
    "d3d12.lib"
    "dxgi.lib"
)

include(FetchContent)
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.16.0
)
FetchContent_MakeAvailable(EnTT)
target_link_libraries(TriKitCore PRIVATE EnTT::EnTT)