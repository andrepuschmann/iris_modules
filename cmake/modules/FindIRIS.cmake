# - Try to find iris - the software radio framework
# Once done this will define
#  IRIS_FOUND - System has iris
#  IRIS_INCLUDE_DIRS - The iris include directories
#  IRIS_DEFINITIONS - Compiler switches required for using iris

find_package(PkgConfig)
pkg_check_modules(PC_IRIS iris_core)
set(IRIS_DEFINITIONS ${PC_IRIS_CFLAGS_OTHER})

find_path(IRIS_INCLUDE_DIR 
            NAMES irisapi/Version.h
            HINTS ${PC_IRIS_INCLUDEDIR} ${PC_IRIS_INCLUDE_DIRS} $ENV{IRIS_DIR}/include
            PATHS /usr/local/include 
                  /usr/include )

set(IRIS_INCLUDE_DIRS ${IRIS_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set IRIS_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(IRIS DEFAULT_MSG IRIS_INCLUDE_DIR)

mark_as_advanced(IRIS_INCLUDE_DIR )
