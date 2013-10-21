project(1_4)
cmake_minimum_required(VERSION 2.8)


set(PROJECT 1_4)
set(MAIN_SOURCES 1_4.cpp)

add_executable(${PROJECT} ${MAIN_SOURCES})
add_definitions( -DSOURCE_DIR="${CMAKE_SOURCE_DIR}" )
