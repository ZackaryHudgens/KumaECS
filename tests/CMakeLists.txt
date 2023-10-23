add_executable(tests main.cpp)
target_include_directories(tests PUBLIC ../include)

install(TARGETS tests DESTINATION bin)
