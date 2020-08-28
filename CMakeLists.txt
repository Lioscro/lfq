cmake_minimum_required (VERSION 3.1)
project (lfq)

# Build options
option(Test "Test" OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RELEASE")
endif()

# Build config
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror)
if(CMAKE_BUILD_TYPE EQUAL "RELEASE")
  add_compile_options(-O3)
else()
  add_compile_options(-Og)
endif()

# Boost
find_package(Boost REQUIRED COMPONENTS iostreams)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
if(Test)
  add_subdirectory(tests)
endif()
