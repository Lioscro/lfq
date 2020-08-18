cmake_minimum_required (VERSION 3.1)
project (lfq)

# Build config
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Build options
option(Test "Test" OFF)

add_subdirectory(src)
if(Test)
  add_subdirectory(tests)
endif()
