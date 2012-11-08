MESSAGE(STATUS "  Processing usrpuhdrx.")

########################################################################
# Add includes and dependencies
########################################################################
FIND_PACKAGE( UHD )

########################################################################
# Build the library from source files
########################################################################
SET(sources
	UsrpUhdRxComponent.cpp
)

IF (UHD_FOUND)
  INCLUDE_DIRECTORIES(${UHD_INCLUDE_DIRS})
  ADD_LIBRARY(comp_gpp_pn_usrpuhdrx SHARED ${sources})
  TARGET_LINK_LIBRARIES(comp_gpp_pn_usrpuhdrx ${UHD_LIBRARIES})
  SET_TARGET_PROPERTIES(comp_gpp_pn_usrpuhdrx PROPERTIES OUTPUT_NAME "usrpuhdrx")
  IRIS_INSTALL(comp_gpp_pn_usrpuhdrx)
  IRIS_APPEND_INSTALL_LIST(usrpuhdrx)
ELSE (UHD_FOUND)
  IRIS_APPEND_NOINSTALL_LIST(usrpuhdrx)
ENDIF (UHD_FOUND)
