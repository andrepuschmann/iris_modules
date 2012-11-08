# - Try to find uhd - the universal hardware driver for ettus rf front-ends
# Once done this will define
#  UHD_FOUND - System has uhd
#  UHD_INCLUDE_DIRS - The uhd include directories
#  UHD_LIBRARIES - The libraries needed to use uhd
#  UHD_DEFINITIONS - Compiler switches required for using uhd

find_package(PkgConfig)
pkg_check_modules(PC_UHD uhd)
set(UHD_DEFINITIONS ${PC_UHD_CFLAGS_OTHER})

find_path(UHD_INCLUDE_DIR 
            NAMES uhd/config.hpp
            HINTS ${PC_UHD_INCLUDEDIR} ${PC_UHD_INCLUDE_DIRS} $ENV{UHD_DIR}/include
            PATHS /usr/local/include 
                  /usr/include )

find_library(UHD_LIBRARY 
            NAMES uhd
            HINTS ${PC_UHD_LIBDIR} ${PC_UHD_LIBRARY_DIRS} $ENV{UHD_DIR}/lib
            PATHS /usr/local/lib
                  /usr/lib)

set(UHD_LIBRARIES ${UHD_LIBRARY} )
set(UHD_INCLUDE_DIRS ${UHD_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBUHD_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(uhd  DEFAULT_MSG
                                  UHD_LIBRARY UHD_INCLUDE_DIR)

mark_as_advanced(UHD_INCLUDE_DIR UHD_LIBRARY )
