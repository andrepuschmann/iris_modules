#include "LineplotEvents.h"

const QEvent::Type PlotLineEvent::type = static_cast<QEvent::Type>(10001);

PlotLineEvent::PlotLineEvent(double* dataPoints,
                             int numPoints)
  : QEvent(QEvent::Type(type))
{
  dataPoints_ = new double[numPoints];
  numPoints_ = numPoints;
  memcpy(dataPoints_, dataPoints, numPoints*sizeof(double));
}

PlotLineEvent::PlotLineEvent(float* dataPoints,
                             int numPoints)
  : QEvent(QEvent::Type(type))
{
  dataPoints_ = new double[numPoints];
  numPoints_ = numPoints;

  for(int i=0;i<numPoints_;i++)
  {
    dataPoints_[i] = dataPoints[i];
  }
}

PlotLineEvent::~PlotLineEvent()
{
  delete[] dataPoints_;
}
