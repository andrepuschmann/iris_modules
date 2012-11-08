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

ADD_LIBRARY(comp_gpp_pn_example SHARED ${sources})
SET_TARGET_PROPERTIES(comp_gpp_pn_example PROPERTIES OUTPUT_NAME "example")
IRIS_INSTALL(comp_gpp_pn_example)
IRIS_APPEND_INSTALL_LIST(example)
