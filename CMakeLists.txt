cmake_minimum_required(VERSION 3.20)
project(Aetheris LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---------- Raylib 5.5 ----------
set(RAYLIB_VERSION 5.5)

include(FetchContent)
FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(OPENGL_VERSION "4.3" CACHE STRING "Force OpenGL 4.3 context for Raylib" FORCE)
FetchContent_MakeAvailable(raylib)

# ---------- Executable ----------
add_executable(${PROJECT_NAME}
    src/main.cpp
    src/core/log.cpp
    src/core/time.cpp
    src/core_math/bounds.cpp
    src/core_math/geometry.cpp
    src/core_math/projection.cpp
    src/input/input.cpp
    src/world/particle_system.cpp
    src/world/world.cpp
    src/rendering/renderer.cpp
    src/rendering/grid_renderer.cpp
    src/rendering/shader/shader.cpp
    src/rendering/shader/compute_shader.cpp
    src/rendering/gpu/gpu_particle_buffer.cpp
    src/viewport/viewport.cpp
    src/spawning/spawn_system.cpp
    
    # Editor
    src/editor/theme/theme.cpp
    src/editor/theme/widgets.cpp
    src/editor/panels/viewport_panel.cpp
    src/editor/panels/toolbar_panel.cpp
    src/editor/panels/left_sidebar.cpp
    src/editor/panels/right_sidebar.cpp
    src/editor/panels/bottom_panel.cpp
    src/editor/editor.cpp

    # Application
    src/application/application.cpp

    # Platform
    src/platform/dpi_aware.cpp

    # Third Party Dependencies
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/rlImGui/rlImGui.cpp
    third_party/implot/implot.cpp
    third_party/implot/implot_items.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE 
    src 
    third_party/imgui 
    third_party/rlImGui 
    third_party/implot
)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
