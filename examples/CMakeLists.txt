cmake_minimum_required(VERSION 3.1)

project(examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
    MINGW_SDK_INIT
    UNICODE
    NANOVG_GL2_IMPLEMENTATION)

add_executable(mainwindow mainwindow.cpp)

target_link_libraries(mainwindow glew32  ${OPENGL_LIBRARIES})

