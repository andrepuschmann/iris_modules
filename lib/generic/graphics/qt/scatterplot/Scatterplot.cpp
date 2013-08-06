#include "Scatterplot.h"
#include "ScatterplotWrapper.h"

Scatterplot::Scatterplot()
{
  plot_ = new ScatterplotWrapper;
}

Scatterplot::~Scatterplot()
{
  delete plot_;
}

void Scatterplot::plotNewData(std::complex<float>* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void Scatterplot::plotNewData(std::complex<double>* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void Scatterplot::setTitle(std::string title)
{
  plot_->setTitle(title);
}

void Scatterplot::setAxes(double xMin, double xMax,
                          double yMin, double yMax)
{
  plot_->setAxes(xMin, xMax, yMin, yMax);
}

void Scatterplot::setAxisLabels(std::string xLabel, std::string yLabel)
{
  plot_->setAxisLabels(xLabel, yLabel);
}

