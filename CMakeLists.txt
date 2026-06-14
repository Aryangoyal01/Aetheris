cmake_minimum_required(VERSION 3.10)
project(AetherisEngine)

# Use modern C++ standards
set(CMAKE_CXX_STANDARD 17)

# Tell the compiler where to find your header (.h) files
include_directories(include)

# Gather your C++ source (.cpp) files
set(SOURCES 
    src/main.cpp 
    src/network.cpp 
    src/ui.cpp
)

# Create the executable
add_executable(AetherisEngine ${SOURCES})

# Link libraries: raylib (raw name for MSYS2) and ws2_32 (Windows networking)
target_link_libraries(AetherisEngine raylib ws2_32)