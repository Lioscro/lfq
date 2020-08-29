cmake_minimum_required (VERSION 3.1)
project (lfq)

# Build options
option(Test "Test" OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RELEASE")
endif()

# Build config.
# Always build statically-linked version.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-fvisibility=hidden)
# Windows MSVC doesn't support -Werror
if(NOT MSVC)
   add_compile_options(-Wall -Werror)
endif()
if(NOT Test)
  set(BUILD_SHARED_LIBS OFF)
  set(Boost_USE_STATIC_LIBS ON)
  if(UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
  endif()
  add_compile_options(-O3)
endif()

# zlib
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

# Boost
find_package(Boost REQUIRED COMPONENTS iostreams)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
if(Test)
  add_subdirectory(tests)
endif()
