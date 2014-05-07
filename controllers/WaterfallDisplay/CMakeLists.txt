#
# Copyright 2012-2013 The Iris Project Developers. See the
# COPYRIGHT file at the top-level directory of this distribution
# and at http://www.softwareradiosystems.com/iris/copyright.html.
#
# This file is part of the Iris Project.
#
# Iris is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# Iris is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# A copy of the GNU Lesser General Public License can be found in
# the LICENSE file in the top-level directory of this distribution
# and at http://www.gnu.org/licenses/.
#

MESSAGE(STATUS "  Processing waterfalldisplay.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	WaterfallDisplayController.cpp
)

IF(IRIS_HAVE_REALPLOT)
    # Static library to be used in tests
    ADD_LIBRARY(controller_waterfalldisplay_static STATIC ${sources})
    TARGET_LINK_LIBRARIES(controller_waterfalldisplay_static waterfallplot)

    # Shared library to be used in radios
    ADD_LIBRARY(controller_waterfalldisplay SHARED ${sources})
    TARGET_LINK_LIBRARIES(controller_waterfalldisplay waterfallplot)

    SET_TARGET_PROPERTIES(controller_waterfalldisplay PROPERTIES OUTPUT_NAME "waterfalldisplay")
    IRIS_INSTALL(controller_waterfalldisplay)
    IRIS_APPEND_INSTALL_LIST("waterfalldisplay")
    ADD_SUBDIRECTORY(test)
ELSE(IRIS_HAVE_REALPLOT)
    IRIS_APPEND_NOINSTALL_LIST("waterfalldisplay")
ENDIF(IRIS_HAVE_REALPLOT)
