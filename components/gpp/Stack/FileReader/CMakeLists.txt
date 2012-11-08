MESSAGE(STATUS "  Processing filereader.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	FileReaderComponent.cpp
)

# Targets must be globally unique for cmake 
ADD_LIBRARY(comp_gpp_stack_filereader SHARED ${sources})
SET_TARGET_PROPERTIES(comp_gpp_stack_filereader PROPERTIES OUTPUT_NAME "filereader")
IRIS_INSTALL(comp_gpp_stack_filereader)
IRIS_APPEND_INSTALL_LIST("filereader")
