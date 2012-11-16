set(DOWNLOAD_DIR ${EXTERNALS_DIR}/rapidjson)

if (DOWNLOAD_EXTERNALS)

  # Configure GoogleTest library
  # ----------------------------
  # checkout and build GTest 1.6.0 automatically
  #set(GTEST_EPBASE ${PROJECT_BINARY_DIR}/ext/gtest)


  ExternalProject_Add(rapid_json
    SVN_REPOSITORY "http://rapidjson.googlecode.com/svn/trunk"
    PREFIX ${DOWNLOAD_DIR}
    STAMP_DIR ${EXTERNALS_DIR}/stamp/rapidjson
    SOURCE_DIR ${DOWNLOAD_DIR}
    BINARY_DIR ${EXTERNALS_DIR}/bin/rapidjson
    DOWNLOAD_DIR ${DOWNLOAD_DIR}
    TMP_DIR ${EXTERNALS_DIR}/tmp/rapidjsons
    UPDATE_COMMAND "" # skip update
    CONFIGURE_COMMAND "" # skip configure
    BUILD_COMMAND "" # skip build
    INSTALL_COMMAND "" # skip install
  )

endif ()

set(RAPIDJSON_INCLUDE_DIRS ${DOWNLOAD_DIR}/include)
