cmake_minimum_required(VERSION 3.10)
project(KumaECS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tests tests/main.cpp)
target_include_directories(tests PUBLIC include)

install(TARGETS tests DESTINATION ${CMAKE_SOURCE_DIR}/install)
