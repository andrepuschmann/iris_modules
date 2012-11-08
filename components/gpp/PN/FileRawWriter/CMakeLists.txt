MESSAGE(STATUS "  Processing filerawwriter.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	FileRawWriterComponent.cpp
)

ADD_LIBRARY(comp_gpp_pn_filerawwriter SHARED ${sources})
SET_TARGET_PROPERTIES(comp_gpp_pn_filerawwriter PROPERTIES OUTPUT_NAME "filerawwriter")
IRIS_INSTALL(comp_gpp_pn_filerawwriter)
IRIS_APPEND_INSTALL_LIST(filerawwriter)
