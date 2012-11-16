set(DOWNLOAD_DIR ${EXTERNALS_DIR}/gtest)

if (DOWNLOAD_EXTERNALS)

  # Configure GoogleTest library
  # ----------------------------
  # checkout and build GTest 1.6.0 automatically
  #set(GTEST_EPBASE ${PROJECT_BINARY_DIR}/ext/gtest)
  ExternalProject_Add(gtest_1.6
    SVN_REPOSITORY "http://googletest.googlecode.com/svn/tags/release-1.6.0"
    SVN_TRUST_CERT 1
    PREFIX ${DOWNLOAD_DIR}
    DOWNLOAD_DIR ${DOWNLOAD_DIR}
    STAMP_DIR ${DOWNLOAD_DIR}/stamp
    SOURCE_DIR ${DOWNLOAD_DIR}/gtest
    BINARY_DIR ${DOWNLOAD_DIR}/bin
    UPDATE_COMMAND "" # don't update, i.e., always use the same version
    BUILD_COMMAND make
    INSTALL_COMMAND "" # skip install
  )

endif ()

set(GTEST_INCLUDE_DIRS ${DOWNLOAD_DIR}/gtest/include)
set(GTEST_LIB_DIRS ${DOWNLOAD_DIR}/bin)
set(GTEST_LIBS gtest gtest_main pthread)
