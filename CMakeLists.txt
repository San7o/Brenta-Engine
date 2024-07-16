cmake_minimum_required(VERSION 3.29.2)
project("ECS Engine")

# src/sources/*.c src/sources/*.cpp

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(include src/headers)

link_directories(lib)

file(GLOB SOURCES "src/sources/*.c" "src/sources/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main pthread glfw GL dl m stdc++ assimp)

target_compile_options(main PRIVATE -Wall)
