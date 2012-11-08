########################################################################
# Add an install dir for components
########################################################################
SET(COMPONENTS_GPP_DIR ${COMPONENTS_DIR}/gpp)

########################################################################
# Add the subdirectories
########################################################################
# Recurse into subdirectories. This does not actually cause another cmake 
# executable to run. The same process will walk through the project's 
# entire directory structure.
ADD_SUBDIRECTORY(gpp)
