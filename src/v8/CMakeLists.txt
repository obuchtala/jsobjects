include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

add_library(jsobjects_v8 STATIC
  ${PROJECT_SOURCE_DIR}/include/jsobjects.hpp
  ${PROJECT_SOURCE_DIR}/include/jsobjects_v8.hpp
  jsobjects_v8.cxx
)
