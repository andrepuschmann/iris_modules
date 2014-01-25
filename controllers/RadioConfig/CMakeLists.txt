MESSAGE(STATUS "  Processing radioconfig.")

########################################################################
# Add includes and dependencies
########################################################################
FIND_PACKAGE( OSPECORR )

########################################################################
# Build the library from source files
########################################################################
SET(sources
    RadioConfigController.cpp
)

IF (OSPECORR_FOUND)
  ADD_LIBRARY(controller_radioconfig SHARED ${sources})
  TARGET_LINK_LIBRARIES (controller_radioconfig ${OSPECORR_LIBS})
  SET_TARGET_PROPERTIES(controller_radioconfig PROPERTIES OUTPUT_NAME "radioconfig")
  SET_TARGET_PROPERTIES(controller_radioconfig PROPERTIES DEFINE_SYMBOL "IRIS_CORE_DLL_EXPORTS")
  IRIS_INSTALL(controller_radioconfig)
  IRIS_APPEND_INSTALL_LIST("radioconfig")
ELSE (OSPECORR_FOUND)
  IRIS_APPEND_NOINSTALL_LIST(radioconfig)
ENDIF (OSPECORR_FOUND)
