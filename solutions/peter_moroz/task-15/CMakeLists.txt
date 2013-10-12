project(task-15)
cmake_minimum_required(VERSION 2.8)

set(PROJECT task-15)
set(HEADERS "")
set(SOURCES "")
set(MAIN_SOURCES task-15.cpp)
set(PRIVATE_CLASSES EarthSurface Island)
set(HEADERS_ONLY Point)

foreach(class ${PRIVATE_CLASSES})
    LIST(APPEND SOURCES ${class}.cpp)
    LIST(APPEND HEADERS ${class}.h)
endforeach()

foreach(class ${HEADERS_ONLY})
   LIST(APPEND HEADERS ${class}.h)
endforeach()

add_executable(${PROJECT} ${MAIN_SOURCES} ${SOURCES})
