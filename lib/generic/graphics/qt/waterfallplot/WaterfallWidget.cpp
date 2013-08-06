#include "WaterfallWidget.h"
#include "Spectrogramplot.h"
#include "common/Lineplot.h"
#include "common/Events.h"

#include <qlayout.h>
#include <algorithm>
#include <boost/lambda/lambda.hpp>

using namespace std;
namespace bl = boost::lambda;


WaterfallWidget::WaterfallWidget(int numDataPoints, int numRows, QWidget *parent)
  :QWidget(parent)
{
  p_ = new Lineplot();
  s_ = new Spectrogramplot(numDataPoints, numRows);

  QVBoxLayout* vLayout1 = new QVBoxLayout(this);
  vLayout1->addWidget(p_);vLayout1->setStretch(0,1);
  vLayout1->addWidget(s_);vLayout1->setStretch(1,3);

  numPoints_ = numDataPoints;
  data_ = new double[numPoints_];
}

WaterfallWidget::~WaterfallWidget()
{
  delete p_;
  delete s_;
}

void WaterfallWidget::customEvent( QEvent * e )
{
  if(e->type() == RealDataEvent::type)
  {
    RealDataEvent* dataEvent = (RealDataEvent*)e;
    plotData(dataEvent);
  }
}

void WaterfallWidget::setWidgetTitle(QString title)
{
  setWindowTitle(title);
}

void WaterfallWidget::setPlotAxes(double xMin, double xMax,
                                  double yMin, double yMax,
                                  double zMin, double zMax)
{
  p_->setXAxisRange(xMin, xMax);
  s_->setPlotAxes(xMin, xMax, yMin, yMax, zMin, zMax);
}

void WaterfallWidget::plotData(RealDataEvent* e)
{
  if(e->numPoints_ != numPoints_)
  {
    numPoints_ = e->numPoints_;
    delete [] data_;
    data_ = new double[numPoints_];
  }

  memcpy(data_, e->dataPoints_, numPoints_*sizeof(double));

  p_->plotData(data_, numPoints_);
  s_->plotData(data_, numPoints_);
}
