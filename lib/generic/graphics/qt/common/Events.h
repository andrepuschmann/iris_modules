#ifndef EVENTS_H
#define EVENTS_H

#include <QEvent>
#include <complex>

class RealDataEvent
  : public QEvent
{
public:
  const static QEvent::Type type;

  RealDataEvent(double* dataPoints,
                int numPoints);
  RealDataEvent(float* dataPoints,
                int numPoints);
  virtual ~RealDataEvent();

  double* dataPoints_;
  int numPoints_;
};

class ComplexDataEvent
  : public QEvent
{
public:
  const static QEvent::Type type;

  ComplexDataEvent(std::complex<double>* dataPoints,
                   int numPoints);
  ComplexDataEvent(std::complex<float>* dataPoints,
                   int numPoints);
  virtual ~ComplexDataEvent();

  std::complex<double>* dataPoints_;
  int numPoints_;
};

#endif // EVENTS_H
