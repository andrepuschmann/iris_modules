########################################################################
# Add an install dir for components
########################################################################
SET(COMPONENTS_GPP_PN_DIR ${COMPONENTS_GPP_DIR}/pn)
SET(COMPONENTS_GPP_STACK_DIR ${COMPONENTS_GPP_DIR}/stack)

########################################################################
# Add the subdirectories
########################################################################
# Recurse into subdirectories. This does not actually cause another cmake 
# executable to run. The same process will walk through the project's 
# entire directory structure.
ADD_SUBDIRECTORY(PN)
ADD_SUBDIRECTORY(Stack)
