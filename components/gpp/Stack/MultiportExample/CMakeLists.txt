MESSAGE(STATUS "  Processing multiportexample.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	MultiportExampleComponent.cpp
)

# Targets must be globally unique for cmake 
ADD_LIBRARY(comp_gpp_stack_multiportexample SHARED ${sources})
SET_TARGET_PROPERTIES(comp_gpp_stack_multiportexample PROPERTIES OUTPUT_NAME "multiportexample")
IRIS_INSTALL(comp_gpp_stack_multiportexample)
IRIS_APPEND_INSTALL_LIST("multiportexample")
