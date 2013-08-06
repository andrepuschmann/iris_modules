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
  QT4_WRAP_CPP(MOCSrcs Spectrogramplot.h WaterfallWidget.h WaterfallplotWrapper.h)
  ADD_LIBRARY(waterfallplot Spectrogram.cpp Spectrogramplot.cpp WaterfallWidget.cpp WaterfallplotWrapper.cpp Waterfallplot.cpp ${MOCSrcs})
  TARGET_LINK_LIBRARIES(waterfallplot ${QT_LIBRARIES} ${QWT_LIBRARIES} lineplot)
  IRIS_SET_PIC(waterfallplot)
  ADD_SUBDIRECTORY(test)
ENDIF(QT4_FOUND AND QWT_FOUND)
