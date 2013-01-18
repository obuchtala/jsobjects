enable_testing()

if (ENABLE_JSC)
	add_subdirectory(jsc)
endif()

if (ENABLE_CPP)
	add_subdirectory(cpp)
endif()
