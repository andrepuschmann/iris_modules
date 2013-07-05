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
  QT4_WRAP_CPP(MOCSrcs Lineplot.h Spectrogramplot.h WaterfallWidget.h WaterfallplotWrapper.h)
  ADD_LIBRARY(waterfallplot Lineplot.cpp Spectrogram.cpp Spectrogramplot.cpp WaterfallWidget.cpp WaterfallplotWrapper.cpp WaterfallplotEvents.cpp Waterfallplot.cpp ${MOCSrcs})
  TARGET_LINK_LIBRARIES(waterfallplot ${QT_LIBRARIES} ${QWT_LIBRARIES})
  IRIS_SET_PIC(waterfallplot)
  ADD_SUBDIRECTORY(test)
ENDIF(QT4_FOUND AND QWT_FOUND)
