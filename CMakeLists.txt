cmake_minimum_required(VERSION 3.14)
project(KumaECS)

add_subdirectory(src)

option(BUILD_TESTS "Build the unit tests." ON)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif(BUILD_TESTS)