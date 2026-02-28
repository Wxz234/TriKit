cmake_minimum_required(VERSION 3.16)
project(TriKit VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(TriKitCore STATIC
    Engine/Core/Engine.cpp
    Engine/Core/Engine.h
)

add_executable(BasicExample WIN32
    Examples/BasicExample.cpp
)

target_link_libraries(BasicExample PRIVATE
    TriKitCore
    d3d12.lib
    dxgi.lib
)