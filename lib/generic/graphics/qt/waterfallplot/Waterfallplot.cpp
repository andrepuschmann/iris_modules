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

void Waterfallplot::plotNewData(float* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void Waterfallplot::plotNewData(double* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void Waterfallplot::setTitle(std::string title)
{
  plot_->setTitle(title);
}

void Waterfallplot::setAxes(double xMin, double xMax,
                            double yMin, double yMax,
                            double zMin, double zMax)
{
  plot_->setAxes(xMin, xMax, yMin, yMax, zMin, zMax);
}
