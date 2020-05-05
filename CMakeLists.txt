cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(vgui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
    MINGW_SDK_INIT
    UNICODE
    NANOVG_GLEW
    NANOVG_GL2_IMPLEMENTATION)

option(VGUI_BUILD_TESTS "Build the tests using GoogleTest"  OFF)

# Always build dll
set(BUILD_SHARED_LIBS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Directory where exe,dll and static library saved
set(VGUI_BINARY_DIR ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${VGUI_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${VGUI_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${VGUI_BINARY_DIR})

# Find dependencies
find_package(opengl REQUIRED)
find_package(glew REQUIRED)
find_package(glfw3 REQUIRED)
find_package(gtest REQUIRED)

# Include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 3dparty
message(STATUS "GLFW3 include directory: '${GLFW_INCLUDE_DIRECTORIES}'")
include_directories(${GLFW_INCLUDE_DIRECTORIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3dparty/entt/src)

# Examples
add_subdirectory(examples)

# Tests
if(VGUI_BUILD_TESTS)
add_subdirectory(tests/gtest)
endif()

# Export INCLUDE_DIRECTORIES to external projects
get_property(INCLUDEDIRS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
set(VGUI_INCLUDE_DIRS ${INCLUDEDIRS} PARENT_SCOPE)

foreach(dir ${VGUI_INCLUDE_DIRS})
  message(STATUS "Vgui include directory: '${dir}'")
endforeach()

