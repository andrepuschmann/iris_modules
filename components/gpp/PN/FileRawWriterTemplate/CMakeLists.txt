MESSAGE(STATUS "  Processing filerawwritertemplate.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	FileRawWriterTemplateComponent.cpp
)

ADD_LIBRARY(comp_gpp_pn_filerawwritertemplate SHARED ${sources})
SET_TARGET_PROPERTIES(comp_gpp_pn_filerawwritertemplate PROPERTIES OUTPUT_NAME "filerawwritertemplate")
IRIS_INSTALL(comp_gpp_pn_filerawwritertemplate)
IRIS_APPEND_INSTALL_LIST(filerawwritertemplate)
