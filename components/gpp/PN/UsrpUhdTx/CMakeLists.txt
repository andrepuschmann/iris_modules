MESSAGE(STATUS "  Processing usrpuhdtx.")

########################################################################
# Add includes and dependencies
########################################################################
FIND_PACKAGE( UHD )

########################################################################
# Build the library from source files
########################################################################
SET(sources
	UsrpUhdTxComponent.cpp
)

IF (UHD_FOUND)
  INCLUDE_DIRECTORIES(${UHD_INCLUDE_DIRS})
  ADD_LIBRARY(comp_gpp_pn_usrpuhdtx SHARED ${sources})
  TARGET_LINK_LIBRARIES(comp_gpp_pn_usrpuhdtx ${UHD_LIBRARIES})
  SET_TARGET_PROPERTIES(comp_gpp_pn_usrpuhdtx PROPERTIES OUTPUT_NAME "usrpuhdtx")
  IRIS_INSTALL(comp_gpp_pn_usrpuhdtx)
  IRIS_APPEND_INSTALL_LIST(usrpuhdtx)
ELSE (UHD_FOUND)
  IRIS_APPEND_NOINSTALL_LIST(usrpuhdtx)
ENDIF (UHD_FOUND)
