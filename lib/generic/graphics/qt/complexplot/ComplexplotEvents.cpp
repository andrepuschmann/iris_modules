#include "ComplexplotEvents.h"

using namespace std;

const QEvent::Type PlotComplexEvent::type = static_cast<QEvent::Type>(10002);

PlotComplexEvent::PlotComplexEvent(complex<double>* dataPoints, int numPoints)
  : QEvent(QEvent::Type(type))
{
  dataPoints_ = new complex<double>[numPoints];
  numPoints_ = numPoints;
  memcpy(dataPoints_, dataPoints, numPoints*sizeof(complex<double>));
}

PlotComplexEvent::PlotComplexEvent(complex<float>* dataPoints, int numPoints)
  : QEvent(QEvent::Type(type))
{
  dataPoints_ = new complex<double>[numPoints];
  numPoints_ = numPoints;

  for(int i=0;i<numPoints_;i++)
  {
    dataPoints_[i] = complex<double>(dataPoints[i].real(),
                                     dataPoints[i].imag());
  }
}

PlotComplexEvent::~PlotComplexEvent()
{
  delete[] dataPoints_;
}
