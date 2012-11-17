set(MODULE_NAME new_value)
set(RUN_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/runme.js)

set(WRAPPER ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}_wrap.cxx)
set(INPUT ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_NAME}.i)
set(INPUT_SOURCES
  ${INPUT}
  ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_NAME}.hpp
)

add_swigjs_command(
  INPUT ${INPUT}
  WRAPPER ${WRAPPER}
  DEPENDS ${INPUT_SOURCES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

SET_SOURCE_FILES_PROPERTIES(${WRAPPER} PROPERTIES GENERATED TRUE)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${JS_INCLUDE_DIRS}
  ${jsobjects_INCLUDE_DIRS}
)

add_library(${MODULE_NAME} SHARED
  ${INPUT_SOURCES}
  ${WRAPPER}
)

target_link_libraries(${MODULE_NAME}
  ${JSOBJECTS_LIB}
  ${JS_LIBS}
)

include_directories(
  ${JS_INTERPRETER_DIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../example_info.h.in ${CMAKE_CURRENT_SOURCE_DIR}/example_info.h)

add_executable(${MODULE_NAME}_test
  ../runner.cxx
)

target_link_libraries(${MODULE_NAME}_test
  ${JS_SHELL_LIBS}
  ${JS_LIBS}
)
