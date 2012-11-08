
########################################################################
# Add includes and dependencies
########################################################################


########################################################################
# Build the library from source files
########################################################################
SET(tml_sources
	tml_arithmetics.c
	tml_processing.c
	tml_varithmetics.c
	tml_stochastics.c
)

ADD_LIBRARY(tml SHARED ${tml_sources})
SET_TARGET_PROPERTIES(tml PROPERTIES DEFINE_SYMBOL "IRIS_CORE_DLL_EXPORTS")
