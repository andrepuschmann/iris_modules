MESSAGE(STATUS "  Processing filewriter.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	FileWriterComponent.cpp
)

# Targets must be globally unique for cmake 
ADD_LIBRARY(comp_gpp_stack_filewriter SHARED ${sources})
SET_TARGET_PROPERTIES(comp_gpp_stack_filewriter PROPERTIES OUTPUT_NAME "filewriter")
IRIS_INSTALL(comp_gpp_stack_filewriter)
IRIS_APPEND_INSTALL_LIST("filewriter")
