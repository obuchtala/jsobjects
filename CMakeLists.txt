PROJECT(jsobjects)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/config)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/swig)

set(EXTERNALS_DIR ${PROJECT_SOURCE_DIR}/ext CACHE STRING "Directory where external projects should be downloaded to" )
set(LIBRARY_TYPE "SHARED" CACHE STRING "Type of library to be created (SHARED|STATIC)")
set(ENABLE_JSC OFF CACHE BOOL "If JavascriptCore adapter should be created (ON|OFF)")
set(ENABLE_V8 OFF CACHE BOOL "If V8 adapter should be created (ON|OFF)")
set(ENABLE_CPP OFF CACHE BOOL "If C++ adapter should be created (ON|OFF)")
set(ENABLE_SWIG OFF CACHE BOOL "If C++ adapter should be created (ON|OFF)")
set(SWIG_TARGET "jsc" CACHE STRING "swig javasript target (jsc|v8)")

set(jsobjects_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

# Configure external projects
# ===========================
if (EXTERNALS OR IMPORT)

  set (DOWNLOAD_EXTERNALS ON)

endif()

include(Externals)

if (NOT EXTERNALS OR IMPORT)

  include(ExportTarget)

  if (ENABLE_CPP)
    add_subdirectory(src/cpp)
  endif ()

  if (ENABLE_JSC)
    add_subdirectory(src/jsc)
  endif ()

  if (ENABLE_V8)
    add_subdirectory(src/v8)
  endif ()

  enable_testing()
  add_subdirectory(test)

endif()
