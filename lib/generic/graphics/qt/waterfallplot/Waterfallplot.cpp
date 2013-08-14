#include "Waterfallplot.h"
#include "WaterfallplotWrapper.h"

using namespace std;

Waterfallplot::Waterfallplot(int numDataPoints, int numRows)
{
  plot_ = new WaterfallplotWrapper(numDataPoints, numRows);
}

Waterfallplot::~Waterfallplot()
{
  delete plot_;
}

void Waterfallplot::appendNewData(float* data, int numPoints)
{
  plot_->appendNewData(data, numPoints);
}

void Waterfallplot::appendNewData(double* data, int numPoints)
{
  plot_->appendNewData(data, numPoints);
}

void Waterfallplot::setTitle(std::string title)
{
  plot_->setTitle(title);
}

void Waterfallplot::setPlotXAxisScale(double xMin, double xMax)
{
  plot_->setPlotXAxisScale(xMin, xMax);
}

void Waterfallplot::setSpectrogramXAxisScale(double xMin, double xMax)
{
  plot_->setSpectrogramXAxisScale(xMin, xMax);
}

void Waterfallplot::setPlotYAxisScale(double yMin, double yMax)
{
  plot_->setPlotYAxisScale(yMin, yMax);
}

void Waterfallplot::setSpectrogramYAxisScale(double yMin, double yMax)
{
  plot_->setSpectrogramYAxisScale(yMin, yMax);
}

void Waterfallplot::setSpectrogramZAxisScale(double zMin, double zMax)
{
  plot_->setSpectrogramZAxisScale(zMin, zMax);
}

