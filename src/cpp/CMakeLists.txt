include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${BOOST_INCLUDE_DIRS}
  ${RAPIDJSON_INCLUDE_DIRS}
)

add_library(jsobjects_cpp ${LIBRARY_TYPE}
  ${PROJECT_SOURCE_DIR}/include/jsobjects.hpp
  ${PROJECT_SOURCE_DIR}/include/jsobjects_cpp.hpp
  jsobjects_cpp.cxx
)
