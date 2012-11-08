MESSAGE(STATUS "  Processing example.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	ExampleComponent.cpp
)

# Targets must be globally unique for cmake 
ADD_LIBRARY(comp_gpp_stack_example SHARED ${sources})
SET_TARGET_PROPERTIES(comp_gpp_stack_example PROPERTIES OUTPUT_NAME "example")
IRIS_INSTALL(comp_gpp_stack_example)
IRIS_APPEND_INSTALL_LIST("example")
