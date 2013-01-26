include(ExternalProject)

FIND_PACKAGE(Boost)
if(NOT Boost_FOUND)
  include(BoostMinimal)
endif()

if (ENABLE_TESTS)
  include(GTest-1.6)
endif()

if (ENABLE_CPP)
  include(RapidJson)
endif()

if(ENABLE_JSC)
  include(JavaScriptCore)
endif()

if(ENABLE_V8)
  include(V8)
endif()
