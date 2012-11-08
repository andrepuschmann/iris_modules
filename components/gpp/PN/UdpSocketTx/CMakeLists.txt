MESSAGE(STATUS "  Processing udpsockettx.")

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
	UdpSocketTxComponent.cpp
)

IF (Boost_FOUND)
  ADD_LIBRARY(comp_gpp_pn_udpsockettx SHARED ${sources})
  TARGET_LINK_LIBRARIES(comp_gpp_pn_udpsockettx)
  SET_TARGET_PROPERTIES(comp_gpp_pn_udpsockettx PROPERTIES OUTPUT_NAME "udpsockettx")
  IRIS_INSTALL(comp_gpp_pn_udpsockettx)
  IRIS_APPEND_INSTALL_LIST(udpsockettx)
ELSE (Boost_FOUND)
  IRIS_APPEND_NOINSTALL_LIST(udpsockettx)
ENDIF (Boost_FOUND)
