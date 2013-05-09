#include "Scatterplot.h"
#include "PointplotWrapper.h"

Scatterplot::Scatterplot()
{
  plot_ = new PointplotWrapper;
}

Scatterplot::~Scatterplot()
{
  delete plot_;
}

void Scatterplot::plotNewData(double* real, double* imag, int numPoints)
{
  plot_->plotNewData(real, imag, numPoints);
}

void Scatterplot::plotNewData(float* real, float* imag, int numPoints)
{
  plot_->plotNewData(real, imag, numPoints);
}

void Scatterplot::setAxes(double xMin, double xMax,
                          double yMin, double yMax)
{
  plot_->setAxes(xMin, xMax, yMin, yMax);
}

void Scatterplot::setTitle(std::string title)
{
  plot_->setTitle(title);
}

void Scatterplot::setXLabel(std::string label)
{
  plot_->setXLabel(label);
}

void Scatterplot::setYLabel(std::string label)
{
  plot_->setYLabel(label);
}

