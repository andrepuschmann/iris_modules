MESSAGE(STATUS "  Processing tcpsocketrx.")

########################################################################
# Add includes and dependencies
########################################################################
SET(Boost_ADDITIONAL_VERSIONS "1.42.0" "1.42" "1.43.0" "1.43" "1.44.0" "1.44" "1.45.0" "1.45" "1.46.0" "1.46" "1.47.0" "1.47")
FIND_PACKAGE(Boost 1.36)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

########################################################################
# Build the library from source files
########################################################################
SET(sources
	TcpSocketRxComponent.cpp
)

IF (Boost_FOUND)
  ADD_LIBRARY(comp_gpp_pn_tcpsocketrx SHARED ${sources})
  TARGET_LINK_LIBRARIES(comp_gpp_pn_tcpsocketrx)
  SET_TARGET_PROPERTIES(comp_gpp_pn_tcpsocketrx PROPERTIES OUTPUT_NAME "tcpsocketrx")
  IRIS_INSTALL(comp_gpp_pn_tcpsocketrx)
  IRIS_APPEND_INSTALL_LIST(tcpsocketrx)
ELSE (Boost_FOUND)
  IRIS_APPEND_NOINSTALL_LIST(tcpsocketrx)
ENDIF (Boost_FOUND)
