#ifndef COMPLEXPLOTEVENTS_H
#define COMPLEXPLOTEVENTS_H

#include <QEvent>
#include <complex>

class PlotComplexEvent
  : public QEvent
{
public:
  const static QEvent::Type type;

  PlotComplexEvent(std::complex<double>* dataPoints,
                int numPoints);
  PlotComplexEvent(std::complex<float>* dataPoints,
                int numPoints);
  virtual ~PlotComplexEvent();

  std::complex<double>* dataPoints_;
  int numPoints_;
};

#endif // COMPLEXPLOTEVENTS_H
