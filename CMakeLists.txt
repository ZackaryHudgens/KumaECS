cmake_minimum_required(VERSION 3.10)
project(KumaECS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(KumaECS INTERFACE)

option(BUILD_TESTS "Build the unit tests." ON)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
