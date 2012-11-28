MESSAGE(STATUS "Building kissfft.")

########################################################################
# Build the library from source files
########################################################################
SET(sources
	kiss_fft.c
)

ADD_LIBRARY(kissfft STATIC ${sources})
IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  SET_TARGET_PROPERTIES(kissfft PROPERTIES COMPILE_FLAGS -fPIC)
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
