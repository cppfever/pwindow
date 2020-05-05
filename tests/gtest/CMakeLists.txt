cmake_minimum_required(VERSION 3.1)

project(vgui-tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
    MINGW_SDK_INIT
    UNICODE
    NANOVG_GL2_IMPLEMENTATION)

set(gtest_disable_pthreads ON)#Only lower case works. DON'T MODIFY !!!
set(GTEST_CREATE_SHARED_LIBRARY ON)
set(GTEST_LINKED_AS_SHARED_LIBRARY ON)

add_executable(all-tests
  all-tests.cpp
  #Exception-test.cpp
  #unique_vector-test.cpp
  #RelationTest2.cpp
  #std-roundTest.cpp
  #PointITest.cpp
  #PointFTest.cpp
  #PointDTest.cpp
  )

target_link_libraries(all-tests stdc++fs vgui glew32 gtest gtest_main)
