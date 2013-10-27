project(1_5)
cmake_minimum_required(VERSION 2.8)


set(PROJECT 1_5)
set(MAIN_SOURCES 1_5.cpp)

add_executable(${PROJECT} ${MAIN_SOURCES})
add_definitions( -DSOURCE_DIR="${CMAKE_SOURCE_DIR}" )
