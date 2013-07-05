#include "Complexplot.h"
#include "ComplexplotWrapper.h"

using namespace std;

Complexplot::Complexplot()
{
  plot_ = new ComplexplotWrapper;
}

Complexplot::~Complexplot()
{
  delete plot_;
}

void Complexplot::plotNewData(complex<double>* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void Complexplot::plotNewData(complex<float>* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void Complexplot::setTitle(std::string title)
{
  plot_->setTitle(title);
}

void Complexplot::setAxes(PlotId id, double xMin, double xMax,
                          double yMin, double yMax)
{
  plot_->setAxes(id, xMin, xMax, yMin, yMax);
}

void Complexplot::setXAxisRange(double xMin, double xMax)
{
  plot_->setXAxisRange(xMin, xMax);
}

