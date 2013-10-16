cmake_minimum_required(VERSION 2.8)

set(PROJECTS task-24 task-25 task-26)

foreach(project ${PROJECTS})
  add_subdirectory(${project})
endforeach()
