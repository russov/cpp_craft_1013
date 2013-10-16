set(PROJECT task-24)
set(MAIN_SOURCES ${PROJECT}.cpp)
set(PRIVATE_CLASSES Task24)
set(COMMON_CLASSES Task StringConstants)

set(SOURCES)
set(HEADERS)

foreach(class ${PRIVATE_CLASSES})
  LIST(APPEND SOURCES ${class}.cpp)
  LIST(APPEND HEADERS ${class}.h)
endforeach()

foreach(class ${COMMON_CLASSES})
  LIST(APPEND SOURCES ../common/${class}.cpp)
  LIST(APPEND HEADERS ../common/${class}.h)
endforeach()

add_executable(${PROJECT} ${MAIN_SOURCES} ${SOURCES} ${HEADERS})
