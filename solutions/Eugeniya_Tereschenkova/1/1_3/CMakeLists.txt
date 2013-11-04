project(1_3)
cmake_minimum_required(VERSION 2.8)


set(PROJECT 1_3)
set(MAIN_SOURCES 1_3.cpp)

add_executable(${PROJECT} ${MAIN_SOURCES})
add_definitions( -DSOURCE_DIR="${CMAKE_SOURCE_DIR}" )
