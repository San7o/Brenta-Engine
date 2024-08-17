cmake_minimum_required(VERSION 3.29.2)
project("ECS Engine")

# src/sources/*.c src/sources/*.cpp

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Harfbuzz causes problems when linking
set (CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(include gmtk2024/headers engine/headers)

link_directories(lib)

file(GLOB SOURCES "gmtk2024/sources/**/*.cpp" "gmtk2024/sources/*.cpp" "engine/sources/*.c" "engine/sources/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main pthread glfw GL dl m stdc++fs assimp freetype harfbuzz freetype SDL3 asound)

target_compile_options(main PRIVATE -Wall)
