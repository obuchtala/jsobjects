include(RunSwigJS)

if ("${SWIG_TARGET}" STREQUAL "jsc")

  set (SWIG_JS_DEFINE "SWIG_JSC")
  set (JS_INCLUDE_DIRS ${JSC_INCLUDE_DIRS})
  set (JS_LIBS ${JSC_LIBS})
  set (JSOBJECTS_LIB jsobjects_jsc)
  set (JS_SHELL_LIBS jsc_shell dl)

else ()
  set (SWIG_JS_DEFINE "SWIG_V8")
  set (JS_LIBS v8)
  set (JSOBJECTS_LIB jsobjects_v8)
  set (JS_SHELL_LIBS v8_shell dl)

endif ()

add_subdirectory(value_argument)
add_subdirectory(new_value)
