MESSAGE(STATUS "  Processing filerawreader.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	FileRawReaderComponent.cpp
)

ADD_LIBRARY(comp_gpp_pn_filerawreader SHARED ${sources})
SET_TARGET_PROPERTIES(comp_gpp_pn_filerawreader PROPERTIES OUTPUT_NAME "filerawreader")
IRIS_INSTALL(comp_gpp_pn_filerawreader)
IRIS_APPEND_INSTALL_LIST(filerawreader)
