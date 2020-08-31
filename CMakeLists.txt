cmake_minimum_required (VERSION 3.1)
project (lfq)

################# Build options
option(Test "Test" OFF)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

################# Build config.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror -fvisibility=hidden)
# Statically link for release
if(CMAKE_BUILD_TYPE MATCHES Release)
  set(BUILD_SHARED_LIBS OFF)
  set(Boost_USE_STATIC_LIBS ON)
  if(UNIX AND NOT APPLE)
    # By default, Linux searches for shared object files (.so) first, but
    # we want static.
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
  endif(UNIX AND NOT APPLE)
  add_compile_options(-O3)
endif()

################# Dependencies
# zlib
find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

# Boost
find_package(Boost REQUIRED COMPONENTS iostreams)
include_directories(${Boost_INCLUDE_DIRS})

################# Add subdirs
add_subdirectory(src)
if(Test)
  add_subdirectory(tests)
endif()
