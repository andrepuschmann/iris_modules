#ifndef WATERFALLPLOTEVENTS_H
#define WATERFALLPLOTEVENTS_H

#include <QEvent>
#include <complex>

class PlotWaterfallEvent
  : public QEvent
{
public:
  const static QEvent::Type type;

  PlotWaterfallEvent(double* dataPoints,
                int numPoints);
  PlotWaterfallEvent(float* dataPoints,
                int numPoints);
  virtual ~PlotWaterfallEvent();

  double* dataPoints_;
  int numPoints_;
};

#endif // WATERFALLPLOTEVENTS_H
