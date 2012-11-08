MESSAGE(STATUS "  Processing udpsocketrx.")

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
	UdpSocketRxComponent.cpp
)

IF (Boost_FOUND)
  ADD_LIBRARY(comp_gpp_pn_udpsocketrx SHARED ${sources})
  TARGET_LINK_LIBRARIES(comp_gpp_pn_udpsocketrx)
  SET_TARGET_PROPERTIES(comp_gpp_pn_udpsocketrx PROPERTIES OUTPUT_NAME "udpsocketrx")
  IRIS_INSTALL(comp_gpp_pn_udpsocketrx)
  IRIS_APPEND_INSTALL_LIST(udpsocketrx)
ELSE (Boost_FOUND)
  IRIS_APPEND_NOINSTALL_LIST(udpsocketrx)
ENDIF (Boost_FOUND)
