########################################################################
# Setup Qt and Qwt
########################################################################
FIND_PACKAGE(Qt4 REQUIRED)
IF(QT4_FOUND)
    INCLUDE(${QT_USE_FILE})
ENDIF(QT4_FOUND) 

FIND_PACKAGE(Qwt REQUIRED)
IF(QT4_FOUND AND QWT_FOUND)
    INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIRS})
ENDIF(QT4_FOUND AND QWT_FOUND)
 
########################################################################
# Build our library
########################################################################
IF(QT4_FOUND AND QWT_FOUND)
    SET(sources
      KnobSpin.cpp
      KnobSpin.h
      RfFrontEndGuiWidget.cpp
      RfFrontEndGuiWidget.h
      RfFrontEndGuiWrapper.cpp
      RfFrontEndGuiWrapper.h
      RfFrontEndGui.cpp
      FEGuiParams.h
      FEGuiCallback.h
    )
    QT4_WRAP_CPP(MOCSrcs KnobSpin.h RfFrontEndGuiWidget.h RfFrontEndGuiWrapper.h)
    ADD_LIBRARY(rffrontendgui ${sources} ${MOCSrcs})
    TARGET_LINK_LIBRARIES(rffrontendgui ${QT_LIBRARIES} ${QWT_LIBRARIES})
    IRIS_SET_PIC(rffrontendgui)
    SET(IRIS_HAVE_RFFRONTENDGUI ON CACHE INTERNAL "Has RfFrontEndGui been built?")
    ADD_SUBDIRECTORY(test)
ELSE(QT4_FOUND AND QWT_FOUND)
    SET(IRIS_HAVE_RFFRONTENDGUI OFF CACHE INTERNAL "Has RfFrontEndGui been built?")
ENDIF(QT4_FOUND AND QWT_FOUND)
