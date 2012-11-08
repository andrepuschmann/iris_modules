MESSAGE(STATUS "  Processing matlabtemplate.")

########################################################################
# Add includes and dependencies
########################################################################
FIND_PACKAGE( MATLAB )

########################################################################
# Build the library from source files
########################################################################
SET(sources
	MatlabTemplateComponent.cpp
)

IF (MATLAB_FOUND)
  INCLUDE_DIRECTORIES(${MATLAB_INCLUDE_DIR})
  ADD_LIBRARY(comp_gpp_pn_matlabtemplate SHARED ${sources})
  TARGET_LINK_LIBRARIES(comp_gpp_pn_matlabtemplate ${MATLAB_LIBRARIES})
  SET_TARGET_PROPERTIES(comp_gpp_pn_matlabtemplate PROPERTIES OUTPUT_NAME "matlabtemplate")
  
  # We add an rpath to the library so matlab dependencies will be found
  # while avoiding clashes with system libraries
  SET_TARGET_PROPERTIES(comp_gpp_pn_matlabtemplate PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
  SET_TARGET_PROPERTIES(comp_gpp_pn_matlabtemplate PROPERTIES INSTALL_RPATH ${MATLAB_LIB_DIR})
  IRIS_INSTALL(comp_gpp_pn_matlabtemplate)
  IRIS_APPEND_INSTALL_LIST(matlabtemplate)
ELSE (MATLAB_FOUND)
  IRIS_APPEND_NOINSTALL_LIST(matlabtemplate)
ENDIF (MATLAB_FOUND)
