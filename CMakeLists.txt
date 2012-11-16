PROJECT(jsobjects)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/config)

set(EXTERNALS_DIR ${PROJECT_SOURCE_DIR}/ext CACHE STRING "Directory where external projects should be downloaded to" )

# Configure external projects
# ===========================
if (EXTERNALS)

  set (DOWNLOAD_EXTERNALS ON)

  include(Externals)

else ()

  include(ExportTarget)
  include(ConfigureVariables)

  # include Externals to get variables set
  include(Externals)

  add_subdirectory(src/cpp)
  add_subdirectory(src/jsc)
  add_subdirectory(src/v8)

  enable_testing()
  add_subdirectory(test)

endif()
