#include "PointplotEvents.h"

const QEvent::Type PlotDataEvent::type = static_cast<QEvent::Type>(10000);

PlotDataEvent::PlotDataEvent(double* realPoints,
                             double* imagPoints,
                             int numPoints)
  : QEvent(QEvent::Type(type))
{
  realPoints_ = new double[numPoints];
  imagPoints_ = new double[numPoints];
  numPoints_ = numPoints;
  memcpy(realPoints_, realPoints, numPoints*sizeof(double));
  memcpy(imagPoints_, imagPoints, numPoints*sizeof(double));
}

PlotDataEvent::PlotDataEvent(float* realPoints,
                             float* imagPoints,
                             int numPoints)
  : QEvent(QEvent::Type(type))
{
  realPoints_ = new double[numPoints];
  imagPoints_ = new double[numPoints];
  numPoints_ = numPoints;

  for(int i=0;i<numPoints_;i++)
  {
    realPoints_[i] = realPoints[i];
    imagPoints_[i] = imagPoints[i];
  }
}

PlotDataEvent::~PlotDataEvent()
{
  delete[] realPoints_;
  delete[] imagPoints_;
}
