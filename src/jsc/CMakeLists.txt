include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

add_library(jsobjects_jsc STATIC
  ${PROJECT_SOURCE_DIR}/include/jsobjects.hpp
  ${PROJECT_SOURCE_DIR}/include/jsobjects_jsc.hpp
  jsobjects_jsc.cxx
)
