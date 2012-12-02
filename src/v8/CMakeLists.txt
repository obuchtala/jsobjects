include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${BOOST_INCLUDE_DIRS}
)

add_library(jsobjects_v8 ${LIBRARY_TYPE}
  ${PROJECT_SOURCE_DIR}/include/jsobjects.hpp
  ${PROJECT_SOURCE_DIR}/include/jsobjects_v8.hpp
  jsobjects_v8.cxx
)
