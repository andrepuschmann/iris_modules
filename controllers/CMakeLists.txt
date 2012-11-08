MESSAGE(STATUS "*****************************")
MESSAGE(STATUS "Processing Controllers.")
MESSAGE(STATUS "*****************************")

########################################################################
# Create files to cache names of installed/not installed modules
########################################################################
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/iris_inst)
FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/iris_noinst)

########################################################################
# Helpful Macros
########################################################################
MACRO(IRIS_INSTALL)
	INSTALL(TARGETS ${ARGV} DESTINATION ${CONTROLLERS_DIR})
ENDMACRO(IRIS_INSTALL)

MACRO(IRIS_APPEND_INSTALL_LIST)
    FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/../iris_inst "\n--     ${ARGV}")
ENDMACRO(IRIS_APPEND_INSTALL_LIST)

MACRO(IRIS_APPEND_NOINSTALL_LIST)
    FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/../iris_noinst "\n--     ${ARGV}")
ENDMACRO(IRIS_APPEND_NOINSTALL_LIST)

########################################################################
# Add the subdirectories
########################################################################
# Recurse into subdirectories. This does not actually cause another cmake 
# executable to run. The same process will walk through the project's 
# entire directory structure.
ADD_SUBDIRECTORY(Example)

########################################################################
# List controllers to be installed/not installed
########################################################################
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/iris_inst IRIS_INST_COMPS)
FILE(READ ${CMAKE_CURRENT_BINARY_DIR}/iris_noinst IRIS_NOINST_COMPS)
MESSAGE(STATUS)
MESSAGE(STATUS "  Building Controllers: ${IRIS_INST_COMPS}")
MESSAGE(STATUS)
MESSAGE(STATUS "  Not building Controllers: ${IRIS_NOINST_COMPS}")
MESSAGE(STATUS)
