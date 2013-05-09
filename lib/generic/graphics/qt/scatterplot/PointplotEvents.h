#ifndef POINTPLOTEVENTS_H
#define POINTPLOTEVENTS_H

#include <QEvent>

class PlotDataEvent
  : public QEvent
{
public:
  const static QEvent::Type type;

  PlotDataEvent(double* realPoints,
                double* imagPoints,
                int numPoints);
  PlotDataEvent(float* realPoints,
                float* imagPoints,
                int numPoints);
  virtual ~PlotDataEvent();

  double* realPoints_;
  double* imagPoints_;
  int numPoints_;
};

#endif // POINTPLOTEVENTS_H
