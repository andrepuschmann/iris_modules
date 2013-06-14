#ifndef LINEPLOTEVENTS_H
#define LINEPLOTEVENTS_H

#include <QEvent>

class PlotLineEvent
  : public QEvent
{
public:
  const static QEvent::Type type;

  PlotLineEvent(double* dataPoints,
                int numPoints);
  PlotLineEvent(float* dataPoints,
                int numPoints);
  virtual ~PlotLineEvent();

  double* dataPoints_;
  int numPoints_;
};

#endif // LINEPLOTEVENTS_H
