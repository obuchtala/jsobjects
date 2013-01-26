include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${JSC_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

###################################
# string encoding

set(_TARGET jsc.test.string_encoding)
set(_SHORT string_encoding)
add_executable(${_TARGET}
	string_encoding
)
target_link_libraries(${_TARGET} ${JSC})
add_test(${_TARGET} ${_TARGET})
