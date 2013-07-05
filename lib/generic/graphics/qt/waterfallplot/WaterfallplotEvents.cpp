#include "WaterfallplotEvents.h"

using namespace std;

const QEvent::Type PlotWaterfallEvent::type = static_cast<QEvent::Type>(10003);

PlotWaterfallEvent::PlotWaterfallEvent(double* dataPoints, int numPoints)
  : QEvent(QEvent::Type(type))
{
  dataPoints_ = new double[numPoints];
  numPoints_ = numPoints;
  memcpy(dataPoints_, dataPoints, numPoints*sizeof(double));
}

PlotWaterfallEvent::PlotWaterfallEvent(float* dataPoints, int numPoints)
  : QEvent(QEvent::Type(type))
{
  dataPoints_ = new double[numPoints];
  numPoints_ = numPoints;

  for(int i=0;i<numPoints_;i++)
  {
    dataPoints_[i] = (double)(dataPoints[i]);
  }
}

PlotWaterfallEvent::~PlotWaterfallEvent()
{
  delete[] dataPoints_;
}
