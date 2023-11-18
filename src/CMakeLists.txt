list(APPEND KUMA_SOURCES
  Scene.cpp
)

add_library(KumaECS ${KUMA_SOURCES})
target_include_directories(KumaECS PUBLIC ../include)