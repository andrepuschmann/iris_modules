MESSAGE(STATUS "  Processing energydetector.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
    EnergyDetectorComponent.cpp
)

# Static library to be used in tests
ADD_LIBRARY(comp_gpp_phy_energydetector_static STATIC ${sources})
ADD_LIBRARY(comp_gpp_phy_energydetector SHARED ${sources})

# Set vector optimation flags if configured
if(USE_EXPERIMENTAL_OPTIMIZATION)
  MESSAGE(STATUS "Using experimental compiler flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pipe -march=native -mtune=native -ffast-math -funsafe-math-optimizations -frename-registers -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=1 -Wall" )
endif()

SET_TARGET_PROPERTIES(comp_gpp_phy_energydetector PROPERTIES OUTPUT_NAME "energydetector")
IRIS_INSTALL(comp_gpp_phy_energydetector)
IRIS_APPEND_INSTALL_LIST(energydetector)
