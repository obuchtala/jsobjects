include (CMakeParseArguments)

function(add_swigjs_command)

    set(options)
    set(oneValueArgs WRAPPER INPUT WORKING_DIRECTORY)
    set(multiValueArgs DEPENDS)
    cmake_parse_arguments(RUN_SWIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

  add_custom_command(
    OUTPUT ${RUN_SWIG_WRAPPER}
    DEPENDS ${RUN_SWIG_DEPENDS}
    WORKING_DIRECTORY ${RUN_SWIG_WORKING_DIRECTORY}
    COMMAND ${SWIG_COMMAND} -I${CMAKE_CURRENT_SOURCE_DIR} -I${PROJECT_SOURCE_DIR}/swig
            -D${SWIG_JS_DEFINE}
            -c++ -javascript -${SWIG_TARGET} -o ${RUN_SWIG_WRAPPER} ${RUN_SWIG_INPUT}
    COMMENT "${SWIG_COMMAND} -I${CMAKE_CURRENT_SOURCE_DIR} -c++ -javascript -${SWIG_TARGET} -D${SWIG_JS_DEFINE} -o ${RUN_SWIG_WRAPPER} ${RUN_SWIG_INPUT}"
  )

endfunction()
