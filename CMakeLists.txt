cmake_minimum_required(VERSION 3.10)
project(AetherisEngine)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(src)

add_compile_definitions(GRAPHICS_API_OPENGL_43)

set(SOURCES
    src/main.cpp
    src/network.cpp
    src/ui.cpp
    src/ui_layout.cpp
    src/ui_widgets.cpp
    src/renderer/renderer.cpp
    src/renderer/gpu_allocator.cpp
    src/renderer/compute_pipeline.cpp
    src/renderer/particle_renderer.cpp
    src/renderer/gl_helpers.cpp
)

add_executable(AetherisEngine ${SOURCES})
target_link_libraries(AetherisEngine PRIVATE raylib glfw3 ws2_32 opengl32)