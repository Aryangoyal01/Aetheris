cmake_minimum_required(VERSION 3.10)
project(AetherisEngine)

set(CMAKE_CXX_STANDARD 17)

set(RAYLIB_LOCAL "${CMAKE_SOURCE_DIR}/raylib_src")

include_directories(include)
include_directories(src)
include_directories(${RAYLIB_LOCAL}/src)

link_directories(${RAYLIB_LOCAL}/build/raylib)

set(SOURCES
    src/main.cpp
    src/network.cpp
    src/ui.cpp
    src/renderer/renderer.cpp
    src/renderer/gpu_allocator.cpp
    src/renderer/compute_pipeline.cpp
    src/renderer/particle_renderer.cpp
    src/renderer/gl_helpers.cpp
)

add_executable(AetherisEngine ${SOURCES})

target_compile_definitions(AetherisEngine PRIVATE GRAPHICS_API_OPENGL_43)

target_link_libraries(AetherisEngine raylib ws2_32 opengl32)

add_custom_command(TARGET AetherisEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/shaders"
        "$<TARGET_FILE_DIR:AetherisEngine>/src/shaders"
)
