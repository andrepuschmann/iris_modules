#include "Realplot.h"
#include "RealplotWrapper.h"

Realplot::Realplot()
{
  plot_ = new RealplotWrapper;
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

void Realplot::setTitle(std::string title)
{
  plot_->setTitle(title);
}

void Realplot::setAxes(double xMin, double xMax,
                          double yMin, double yMax)
{
  plot_->setAxes(xMin, xMax, yMin, yMax);
}

void Realplot::setXAxisRange(double xMin, double xMax)
{
  plot_->setXAxisRange(xMin, xMax);
}

void Realplot::setLabels(std::string xLabel, std::string yLabel)
{
  plot_->setAxisLabels(xLabel, yLabel);
}

