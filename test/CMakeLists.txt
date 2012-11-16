include_directories(
  ${SUBSTANCE_EXTENSION_SOURCE_DIR}
  ${GTEST_INCLUDE_DIRS}
  ${BOOST_INCLUDE_DIRS}
  ${jsobjects_INCLUDE_DIRS}
)

link_directories(
  ${GTEST_LIB_DIRS}
)

add_executable(jsobjects.cpp.unit
  jsobjects_cpp_test.cxx
)

target_link_libraries(jsobjects.cpp.unit
  jsobjects_cpp
  ${GTEST_LIBS}
)

add_test(
  NAME jsobjects.cpp.unit
  COMMAND $<TARGET_FILE:jsobjects.cpp.unit>
)

