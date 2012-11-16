include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

add_library(jsobjects_cpp STATIC
  ${PROJECT_SOURCE_DIR}/include/jsobjects.hpp
  ${PROJECT_SOURCE_DIR}/include/jsobjects_cpp.hpp
  jsobjects_cpp.cxx
)
