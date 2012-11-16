PROJECT(jsobjects)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/config)

set(EXTERNALS_DIR ${PROJECT_SOURCE_DIR}/ext CACHE STRING "Directory where external projects should be downloaded to" )
set(jsobjects_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

# Configure external projects
# ===========================
if (EXTERNALS OR IMPORT)

  set (DOWNLOAD_EXTERNALS ON)

endif()

include(Externals)

if (NOT EXTERNALS OR IMPORT)

  include(ExportTarget)

  add_subdirectory(src/cpp)

  if(ENABLE_JAVASCRIPT_CORE)
    add_subdirectory(src/jsc)
  endif()

  if(ENABLE_V8)
    add_subdirectory(src/v8)
  endif()

  enable_testing()
  add_subdirectory(test)

endif()
