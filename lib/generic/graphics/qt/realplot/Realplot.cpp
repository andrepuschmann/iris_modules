#include "Realplot.h"
#include "LineplotWrapper.h"

Realplot::Realplot()
{
  plot_ = new LineplotWrapper;
}

Realplot::~Realplot()
{
  delete plot_;
}

void Realplot::plotNewData(double* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void Realplot::plotNewData(float* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void Realplot::setAxes(double xMin, double xMax,
                          double yMin, double yMax)
{
  plot_->setAxes(xMin, xMax, yMin, yMax);
}

void Realplot::setTitle(std::string title)
{
  plot_->setTitle(title);
}

void Realplot::setXLabel(std::string label)
{
  plot_->setXLabel(label);
}

void Realplot::setYLabel(std::string label)
{
  plot_->setYLabel(label);
}

