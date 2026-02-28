# ====================================================================
# Project:   Brenta Engine
# Author:    Giovanni Santini
# Mail:      giovanni.santini@proton.me
# Github:    @San7o
# License:   MIT
# ====================================================================

cmake_minimum_required(VERSION 3.16)

project(brenta
  VERSION 1.2.0
  DESCRIPTION "Brenta Engine"
  LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # no gnu extensions
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_program(CCACHE_PROGRAM ccache) # use ccache if available
if(CCACHE_PROGRAM)
  message(STATUS "Using ccache: ${CCACHE_PROGRAM}")
  set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()
find_package(OpenGL REQUIRED)

set(BRENTA_DEFINES)

if (WIN32)
  # Put the .exe and .ldd in the same directory so they can be found
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

if(MSVC)
  set(BRENTA_WARNING_FLAGS /W4 /permissive-)
  set(BRENTA_DEBUG_FLAGS /Zi /Od)
  set(BRENTA_RELEASE_FLAGS /O2)
  set(BRENTA_DEBUG_DEFINES _DEBUG)
elseif(MINGW)
  set(BRENTA_WARNING_FLAGS -Wall -Wextra -Wpedantic)
  set(BRENTA_DEBUG_FLAGS -O0 -ggdb)
  set(BRENTA_RELEASE_FLAGS -O3)
  set(BRENTA_DEBUG_DEFINES _DEBUG)
else()
  set(BRENTA_WARNING_FLAGS -Wall -Wextra -Werror -Wpedantic)
  set(BRENTA_DEBUG_FLAGS -O0 -ggdb -fsanitize=address)
  set(BRENTA_RELEASE_FLAGS -O3)
  set(BRENTA_DEBUG_DEFINES _DEBUG)
  set(BRENTA_DEBUG_LINK_FLAGS -fsanitize=address)
endif()

#
# Options
#

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BRENTA_BUILD_EXAMPLES "Build examples" OFF)
option(BRENTA_BUILD_TESTS "Build unit tests" OFF)
option(BRENTA_USE_IMGUI "Enable ImGUI support" ON)
option(BRENTA_USE_ECS "Enable ECS support" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (Debug, Release)" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#
# Dependencies
#

set(BRENTA_DEPS_SOURCES)
set(BRENTA_DEPS_HEADERS)
set(BRENTA_DEPS_INCLUDES)
set(BRENTA_DEPS_LIBS)

macro(register_dependency NAME)
  message(STATUS "Using ${NAME}")
  list(APPEND BRENTA_DEPS_INCLUDES
    ${PROJECT_SOURCE_DIR}/external/${NAME}/include)
  if (BRENTA_BUILD_EXAMPLES OR BRENTA_BUILD_TESTS)
    list(APPEND BRENTA_DEPS_LIBS ${NAME})
    if(NOT TARGET ${NAME})
      add_subdirectory(external/${NAME})
    endif()
  endif()
endmacro()

register_dependency(freetype)
register_dependency(glfw)
register_dependency(glm)
register_dependency(tinyobjloader)

list(APPEND BRENTA_DEPS_INCLUDES
  ${PROJECT_SOURCE_DIR}/external)

if (BRENTA_BUILD_EXAMPLES OR BRENTA_BUILD_TESTS)
  list(APPEND BRENTA_DEPS_SOURCES
    ${PROJECT_SOURCE_DIR}/external/glad/glad.cpp
    ${PROJECT_SOURCE_DIR}/external/miniaudio/miniaudio.c)
  list(APPEND BRENTA_DEPS_HEADERS
    ${PROJECT_SOURCE_DIR}/external/glad/glad.h
    ${PROJECT_SOURCE_DIR}/external/miniaudio/miniaudio.h)
endif()

# ImGUI
if (BRENTA_USE_IMGUI)
  message(STATUS "Using ImGUI")
  list(APPEND BRENTA_DEFINES BRENTA_USE_IMGUI)
  list(APPEND BRENTA_DEPS_INCLUDES
    ${PROJECT_SOURCE_DIR}/external/imgui
    ${PROJECT_SOURCE_DIR}/external/imgui/backends
    ${PROJECT_SOURCE_DIR}/external/imgui/misc/cpp)

  # ImGUI wants X11
  if (NOT WIN32)
    list(APPEND BRENTA_DEPS_LIBS X11)
  endif()
  
  if (BRENTA_BUILD_EXAMPLES OR BRENTA_BUILD_TESTS)
    file(GLOB IMGUI_SOURCES
      "external/imgui/*.cpp"
      "external/imgui/backends/imgui_impl_glfw.cpp"
      "external/imgui/misc/cpp/imgui_stdlib.cpp"
      "external/imgui/backends/imgui_impl_opengl3.cpp"
      "external/imgui/misc/cpp/imgui_stdlib.h")
    list(APPEND BRENTA_DEPS_SOURCES ${IMGUI_SOURCES})
    list(APPEND BRENTA_DEPS_HEADERS "external/imgui/imgui.h")
  endif()
endif()

# Viotecs
if (BRENTA_USE_ECS)
  list(APPEND BRENTA_DEFINES BRENTA_USE_ECS)
  register_dependency(viotecs)
endif()

# Valfuzz
if (BRENTA_BUILD_TESTS)
  message(STATUS "Using valFuzz")
  add_subdirectory(external/valFuzz)
endif()

# Add oak after viotecs and valfuzz
register_dependency(oak)

# Tenno template library
message(STATUS "Using tenno")
list(APPEND BRENTA_DEPS_INCLUDES "external/tenno-tl/include")

list(APPEND BRENTA_DEPS_LIBS OpenGL::GL pthread dl m)

#
# Sources and headers
#

file(GLOB_RECURSE BRENTA_SOURCES "src/*.cpp")
file(GLOB_RECURSE BRENTA_HEADERS "include/brenta/*.hpp")

#
# Targets
#

if (BUILD_SHARED_LIBS)
  message(STATUS "Building shared library")
  add_library(${PROJECT_NAME} SHARED
    ${BRENTA_SOURCES}
    ${BRENTA_HEADERS}
    ${BRENTA_DEPS_SOURCES}
    ${BRENTA_DEPS_HEADERS})
else()
  message(STATUS "Building static library")  
  add_library(${PROJECT_NAME} STATIC
    ${BRENTA_SOURCES}
    ${BRENTA_HEADERS}
    ${BRENTA_DEPS_SOURCES}
    ${BRENTA_DEPS_HEADERS})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${BRENTA_DEPS_INCLUDES})
target_link_libraries(${PROJECT_NAME}
  ${BRENTA_DEPS_LIBS})
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${BRENTA_DEFINES})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Configuring Debug build options...")

  target_compile_definitions(${PROJECT_NAME} PRIVATE
    ${BRENTA_DEBUG_DEFINES})
  target_compile_options(${PROJECT_NAME} PRIVATE
    ${BRENTA_DEBUG_FLAGS}
    ${BRENTA_WARNING_FLAGS})
  if(NOT MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE
      ${BRENTA_DEBUG_LINK_FLAGS})
  endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  message(STATUS "Configuring Release build options...")
  target_compile_options(${PROJECT_NAME} PRIVATE
    ${BRENTA_RELEASE_FLAGS}
    ${BRENTA_WARNING_FLAGS})
endif()

function(add_example NAME)
  message(STATUS "Added example ${NAME}")
  
  add_executable(${NAME}
    ${PROJECT_SOURCE_DIR}/examples/${NAME}.cpp)
  target_include_directories(${NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${BRENTA_DEPS_INCLUDES})
  target_link_libraries(${NAME} PRIVATE
    ${PROJECT_NAME}
    ${BRENTA_DEPS_LIBS})
  target_compile_definitions(${NAME} PRIVATE
    ${BRENTA_DEFINES})
endfunction()

if (BRENTA_BUILD_EXAMPLES)
  message(STATUS "Building examples")
  
  add_example(simple_screen)
  add_example(audio)
  add_example(components)
  add_example(ecs)
  add_example(load_model)
  add_example(load_opengl)
  add_example(logger)
  add_example(mandelbrot-set)
  add_example(particles)
  add_example(text)
  add_example(shadertoy)
  add_example(renderer)
  add_example(vertex_mesh)
  add_example(scene)
  add_example(lights)
  add_example(aircraft)
  add_example(asset_manager)
  add_example(hot_reloading)

  # Demo
  file(GLOB_RECURSE DEMO_SOURCES
    ${PROJECT_SOURCE_DIR}/examples/demo/src/**/*.cpp
    ${PROJECT_SOURCE_DIR}/examples/demo/src/*.cpp)
  add_executable(demo ${DEMO_SOURCES})
  target_include_directories(demo PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/examples/demo/include
    ${BRENTA_DEPS_INCLUDES})
  target_link_libraries(demo PRIVATE
    ${PROJECT_NAME}
    ${BRENTA_DEPS_LIBS})
  target_compile_definitions(demo PRIVATE
    ${BRENTA_DEFINES})

endif()

if (BRENTA_BUILD_TESTS)
  file(GLOB_RECURSE BRENTA_TESTS_SRC
    ${PROJECT_SOURCE_DIR}/tests/**/*.cpp)
  add_executable(test
    ${PROJECT_SOURCE_DIR}/tests/tests.cpp
    ${BRENTA_TESTS_SRC})
  target_include_directories(test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/valFuzz/include
    ${BRENTA_DEPS_INCLUDES})
  target_link_libraries(test PRIVATE
    valfuzz
    ${BRENTA_DEPS_LIBS}
    ${PROJECT_NAME})
  target_compile_definitions(test PRIVATE
    ${BRENTA_DEFINES})
endif()

#
# Installation
#

install(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

#
# Packaging support
#

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

#
# End of CMakeLists.txt
# ====================================================================
