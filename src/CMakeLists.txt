# Find source files
file(GLOB SOURCES *.hpp *.cpp)
list(REMOVE_ITEM SOURCES main.cpp)

# Boost
find_package(Boost REQUIRED COMPONENTS iostreams)
include_directories(${Boost_INCLUDE_DIRS})

add_library(lfq_src ${SOURCES})
target_include_directories(lfq_src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(lfq main.cpp)
target_link_libraries(lfq lfq_src ${Boost_LIBRARIES})
