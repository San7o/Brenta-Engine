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

option(BUILD_MAIN "Build main" ON)
option(BUILD_TESTS "Build unit tests" OFF)

if (BUILD_MAIN AND NOT BUILD_TESTS)
    include_directories(include game/headers engine/headers ecs/headers utils/headers)
    link_directories(lib)
    file(GLOB SOURCES "game/sources/**/*.cpp" "game/sources/*.cpp" "engine/sources/*.c" "engine/sources/*.cpp" "utils/sources/*.cpp" "ecs/sources/*.cpp")
    add_executable(main ${SOURCES})
    target_link_libraries(main pthread glfw GL dl m stdc++fs assimp freetype harfbuzz freetype SDL3 asound)
    target_compile_options(main PRIVATE -Wall)
endif()

if (BUILD_TESTS)
    include_directories(include tests/headers game/headers engine/headers ecs/headers utils/headers )
    link_directories(lib)
    # find_package(Catch2 3 REQUIRED)
    file(GLOB TEST_SOURCES "tests/**/*.cpp" "tests/*.cpp" "engine/sources/*.c" "engine/sources/*.cpp" "utils/sources/*.cpp" "ecs/sources/*.cpp" )
    add_executable(tests ${TEST_SOURCES})
    target_link_libraries(tests pthread glfw GL dl m stdc++fs assimp freetype harfbuzz freetype SDL3 asound)
    target_compile_options(tests PRIVATE -Wall)
    add_custom_target(run_tests COMMAND tests DEPENDS tests)
endif()
