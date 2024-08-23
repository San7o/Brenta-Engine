cmake_minimum_required(VERSION 3.29.2)
project("Brenta Engine")

find_library(asound asound)
find_library(freetype freetype)
find_library(harfbuzz harfbuzz)
find_library(assimp assimp)
find_library(glfw glfw)
find_library(GL GL)
find_library(dl dl)
find_library(m m)
find_library(pthread pthread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Harfbuzz causes problems when linking
set (CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

link_directories(lib)
link_libraries(asound glfw GL dl m stdc++fs assimp freetype harfbuzz SDL3 asound)

option(BUILD_MAIN "Build main" ON)
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

file(GLOB ENGINE_SOURCES "engine/sources/*.c" "engine/sources/*.cpp" "utils/sources/*.cpp" )
file(GLOB ECS_SOURCES "ecs/sources/*.cpp")

if (BUILD_MAIN AND NOT BUILD_TESTS AND NOT BUILD_EXAMPLES)
    include_directories(include game/headers engine/headers ecs/headers utils/headers)
    add_compile_definitions(USE_ECS)
    file(GLOB MAIN_SOURCES "game/sources/**/*.cpp" "game/sources/*.cpp" )
    add_executable(main ${ENGINE_SOURCES} ${ECS_SOURCES} ${MAIN_SOURCES} )
    target_compile_options(main PRIVATE -Wall)
endif()

if (BUILD_TESTS)
    include_directories(include tests/headers game/headers engine/headers ecs/headers utils/headers )
    add_compile_definitions(USE_ECS)
    file(GLOB TEST_SOURCES ${TEST_SOURCES} "tests/**/*.cpp" "tests/*.cpp" )
    add_executable(tests ${TEST_SOURCES} ${ENGINE_SOURCES})
    target_compile_options(tests PRIVATE -Wall)
endif()

if (BUILD_EXAMPLES)
    include_directories(include engine/headers utils/headers )
    add_executable(simple_screen ${ENGINE_SOURCES} "examples/simple_screen.cpp")
    add_executable(load_opengl ${ENGINE_SOURCES} "examples/load_opengl.cpp")
    add_executable(load_model ${ENGINE_SOURCES} "examples/load_model.cpp")
    add_executable(ecs ${ENGINE_SOURCES} ${ECS_SOURCES} "examples/ecs.cpp")
    target_compile_options(ecs PRIVATE -DUSE_ECS)
    target_include_directories(ecs PRIVATE ecs/headers )
    add_executable(particles ${ENGINE_SOURCES} "examples/particles.cpp")
    add_executable(audio ${ENGINE_SOURCES} "examples/audio.cpp")
    add_executable(text ${ENGINE_SOURCES} "examples/text.cpp")
endif()
