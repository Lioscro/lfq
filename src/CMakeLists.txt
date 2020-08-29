file(GLOB SOURCES *.hpp *.cpp)
list(REMOVE_ITEM SOURCES main.cpp)

add_library(lfq_src STATIC ${SOURCES})
target_include_directories(lfq_src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lfq_src PUBLIC ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})

add_executable(lfq main.cpp)
target_link_libraries(lfq lfq_src)
