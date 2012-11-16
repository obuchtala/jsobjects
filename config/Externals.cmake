include(ExternalProject)

include(BoostMinimal)
include(GTest-1.6)

if(ENABLE_JAVASCRIPT_CORE)
  include(JavaScriptCore)
endif()

if(ENABLE_V8)
  include(V8)
endif()
