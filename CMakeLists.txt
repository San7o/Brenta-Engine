cmake_minimum_required(VERSION 3.29.2)
project("Brenta Engine")

set(LIBS_DIR ${CMAKE_SOURCE_DIR}/lib)
link_directories(${LIBS_DIR})
find_library(asound asound REQUIRED)
find_library(freetype freetype REQUIRED)
find_library(harfbuzz harfbuzz REQUIRED)
find_library(assimp assimp PATHS ${CMAKE_SOURCE_DIR}/lib REQUIRED)
find_library(glfw glfw REQUIRED)
find_library(GL GL REQUIRED)
find_library(dl dl REQUIRED)
find_library(m m REQUIRED)
find_library(pthread pthread REQUIRED)

include(cmake/CPM.cmake) # <3

CPMAddPackage(
    NAME oak
    GITHUB_REPOSITORY San7o/oak
    GIT_TAG v1.0.1
    DOWNLOAD_ONLY True
)
CPMUsePackageLock(package-lock.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Harfbuzz causes problems when linking
set (CMAKE_DISABLE_FIND_PACKAGE_HarfBuzz TRUE)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(BRENTA_BUILD_IMGUI "Build with ImGui" ON)
option(BRENTA_BUILD_MAIN "Build main" ON)
option(BRENTA_BUILD_TESTS "Build unit tests" OFF)
option(BRENTA_BUILD_EXAMPLES "Build examples" OFF)
option(BRENTA_BUILD_ECS "Build with ECS" ON)
option(BRENTA_BUILD_STATIC "Build static library" OFF)

set(BRENTA_INCLUDES)
set(BRENTA_COMPILE_OPTIONS)
set(BRENTA_LINK_LIBRARIES)
set(BRENTA_COMPILE_OPTIONS)
set(BRENTA_EXTRA_SOURCES)

list(APPEND BRENTA_INCLUDES include utils/headers engine/headers)
list(APPEND BRENTA_COMPILE_OPTIONS -Wall) # TODO: add more

if (oak_ADDED)
    list(APPEND BRENTA_INCLUDES ${oak_SOURCE_DIR}/include)
    list(APPEND BRENTA_COMPILE_OPTIONS -DOAK_USE_SOCKETS)
    list(APPEND BRENTA_EXTRA_SOURCES ${oak_SOURCE_DIR}/src/oak.cpp)
endif()
if (BRENTA_BUILD_MAIN)
    list(APPEND BRENTA_INCLUDES game/headers)
endif()
if (BRENTA_BUILD_IMGUI)
    list(APPEND BRENTA_INCLUDES imgui imgui/backends
            imgui/misc/cpp)
    list(APPEND BRENTA_COMPILE_OPTIONS -DUSE_IMGUI)
endif()
if (BRENTA_BUILD_ECS)
    list(APPEND BRENTA_INCLUDES ecs/headers)
    list(APPEND BRENTA_COMPILE_OPTIONS -DUSE_ECS)
endif()
if (BRENTA_BUILD_STATIC)
    list(APPEND BRENTA_LINK_LIBRARIES GL dl z m stdc++fs glfw
            asound ${LIBS_DIR}/libassimp.a
            ${LIBS_DIR}/libfreetype.a ${LIBS_DIR}/libharfbuzz.a
            ${LIBS_DIR}/libSDL3.a)
else()
    list(APPEND BRENTA_LINK_LIBRARIES GL dl z m stdc++fs glfw
            asound assimp freetype harfbuzz SDL3)
endif()

file(GLOB BRENTA_IMGUI_SOURCES "imgui/*.cpp" "imgui/backends/imgui_impl_glfw.cpp"
        "imgui/misc/cpp/imgui_stdlib.cpp" "imgui/backends/imgui_impl_opengl3.cpp" )
file(GLOB BRENTA_ENGINE_SOURCES "engine/sources/*.c" "engine/sources/*.cpp" ${BRENTA_EXTRA_SOURCES})
file(GLOB BRENTA_ECS_SOURCES "ecs/sources/*.cpp")
file(GLOB BRENTA_TEST_SOURCES "tests/**/*.cpp" "tests/*.cpp")
file(GLOB BRENTA_MAIN_SOURCES "game/sources/**/*.cpp" "game/sources/*.cpp" )

if (BRENTA_BUILD_MAIN AND NOT BRENTA_BUILD_TESTS AND NOT BRENTA_BUILD_EXAMPLES)
    if (BRENTA_BUILD_ECS)
        add_executable(main ${BRENTA_ENGINE_SOURCES} ${BRENTA_ECS_SOURCES}
                    ${BRENTA_MAIN_SOURCES} ${BRENTA_IMGUI_SOURCES})
    else()
        add_executable(main ${BRENTA_ENGINE_SOURCES} ${BRENTA_MAIN_SOURCES}
                    ${BRENTA_IMGUI_SOURCES} ${BRENTA_IMGUI_SOURCES})
    endif()
    target_include_directories(main PRIVATE ${BRENTA_INCLUDES})
    target_link_libraries(main PRIVATE ${BRENTA_LINK_LIBRARIES})
    target_compile_options(main PRIVATE ${BRENTA_COMPILE_OPTIONS})
endif()

if (BRENTA_BUILD_TESTS)
    add_executable(tests ${BRENTA_TEST_SOURCES} ${BRENTA_ENGINE_SOURCES} ${BRENTA_ECS_SOURCES})
    target_include_directories(tests PRIVATE ${BRENTA_INCLUDES})
    target_link_libraries(tests PRIVATE ${BRENTA_LINK_LIBRARIES})
    target_compile_options(tests PRIVATE ${BRENTA_COMPILE_OPTIONS})
endif()

if (BRENTA_BUILD_EXAMPLES)
    set(BRENTA_EXAMPLES_INCLUDES ${BRENTA_INCLUDES})
    
    add_executable(simple_screen ${BRENTA_ENGINE_SOURCES} "examples/simple_screen.cpp")
    target_include_directories(simple_screen PRIVATE ${BRENTA_EXAMPLES_INCLUDES})
    target_link_libraries(simple_screen PRIVATE ${BRENTA_LINK_LIBRARIES})

    add_executable(logger ${BRENTA_ENGINE_SOURCES} "examples/logger.cpp")
    target_include_directories(logger PRIVATE ${BRENTA_EXAMPLES_INCLUDES})
    target_link_libraries(logger PRIVATE ${BRENTA_LINK_LIBRARIES})

    add_executable(load_opengl ${BRENTA_ENGINE_SOURCES} "examples/load_opengl.cpp")
    target_include_directories(load_opengl PRIVATE ${BRENTA_EXAMPLES_INCLUDES})
    target_link_libraries(load_opengl PRIVATE ${BRENTA_LINK_LIBRARIES})

    add_executable(load_model ${BRENTA_ENGINE_SOURCES} "examples/load_model.cpp")
    target_include_directories(load_model PRIVATE ${BRENTA_EXAMPLES_INCLUDES})
    target_link_libraries(load_model PRIVATE ${BRENTA_LINK_LIBRARIES})

    add_executable(ecs ${BRENTA_ENGINE_SOURCES} ${BRENTA_ECS_SOURCES} "examples/ecs.cpp")
    target_compile_options(ecs PRIVATE -DUSE_ECS)
    target_include_directories(ecs PRIVATE ecs/headers ${BRENTA_EXAMPLES_INCLUDES})
    target_link_libraries(ecs PRIVATE ${BRENTA_LINK_LIBRARIES})
    
    add_executable(particles ${BRENTA_ENGINE_SOURCES} "examples/particles.cpp")
    target_include_directories(particles PRIVATE ${BRENTA_EXAMPLES_INCLUDES})
    target_link_libraries(particles PRIVATE ${BRENTA_LINK_LIBRARIES})
    
    add_executable(audio ${BRENTA_ENGINE_SOURCES} "examples/audio.cpp")
    target_include_directories(audio PRIVATE ${BRENTA_EXAMPLES_INCLUDES})
    target_link_libraries(audio PRIVATE ${BRENTA_LINK_LIBRARIES})
    
    add_executable(text ${BRENTA_ENGINE_SOURCES} "examples/text.cpp")
    target_include_directories(text PRIVATE ${BRENTA_EXAMPLES_INCLUDES})
    target_link_libraries(text PRIVATE ${BRENTA_LINK_LIBRARIES})
    
    add_executable(mandelbrot ${BRENTA_ENGINE_SOURCES} ${BRENTA_IMGUI_SOURCES} "examples/mandelbrot-set.cpp")
    target_compile_options(mandelbrot PRIVATE -DUSE_IMGUI)
    target_include_directories(mandelbrot PRIVATE ${BRENTA_EXAMPLES_INCLUDES})
    target_link_libraries(mandelbrot PRIVATE ${BRENTA_LINK_LIBRARIES})
endif()
