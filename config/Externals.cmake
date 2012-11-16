include(ExternalProject)

include(BoostMinimal)
include(GTest-1.6)
include(RapidJson)

if(ENABLE_JSC)
  include(JavaScriptCore)
endif()

if(ENABLE_V8)
  include(V8)
endif()
