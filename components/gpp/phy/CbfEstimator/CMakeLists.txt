MESSAGE(STATUS "  Processing cbfestimator.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
    CbfEstimatorComponent.cpp
)

# Static library to be used in tests
ADD_LIBRARY(comp_gpp_phy_cbfestimator_static STATIC ${sources})
ADD_LIBRARY(comp_gpp_phy_cbfestimator SHARED ${sources})
SET_TARGET_PROPERTIES(comp_gpp_phy_cbfestimator PROPERTIES OUTPUT_NAME "cbfestimator")
IRIS_INSTALL(comp_gpp_phy_cbfestimator)
IRIS_APPEND_INSTALL_LIST(cbfestimator)
