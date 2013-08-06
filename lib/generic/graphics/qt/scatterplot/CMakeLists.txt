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
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/generic/graphics/qt)
  QT4_WRAP_CPP(MOCSrcs ScatterWidget.h ScatterplotWrapper.h)
  ADD_LIBRARY(scatterplot ScatterWidget.cpp ScatterplotWrapper.cpp Scatterplot.cpp ${MOCSrcs})
  TARGET_LINK_LIBRARIES(scatterplot ${QT_LIBRARIES} ${QWT_LIBRARIES} pointplot)
  IRIS_SET_PIC(scatterplot)
  ADD_SUBDIRECTORY(test)
ENDIF(QT4_FOUND AND QWT_FOUND)
