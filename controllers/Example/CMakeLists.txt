MESSAGE(STATUS "  Processing example.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	ExampleController.cpp
)

# Targets must be globally unique for cmake 
ADD_LIBRARY(controller_example SHARED ${sources})
SET_TARGET_PROPERTIES(controller_example PROPERTIES OUTPUT_NAME "example")
SET_TARGET_PROPERTIES(controller_example PROPERTIES DEFINE_SYMBOL "IRIS_CORE_DLL_EXPORTS")
IRIS_INSTALL(controller_example)
IRIS_APPEND_INSTALL_LIST("example")
