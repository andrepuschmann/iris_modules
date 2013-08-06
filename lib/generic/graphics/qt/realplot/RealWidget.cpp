#include "RealWidget.h"
#include "common/Lineplot.h"
#include "common/Events.h"

#include <qlayout.h>
#include <algorithm>
#include <boost/lambda/lambda.hpp>

using namespace std;
namespace bl = boost::lambda;

RealWidget::RealWidget(QWidget *parent)
  :QWidget(parent)
{
  l_ = new Lineplot();
  QVBoxLayout* vLayout1 = new QVBoxLayout(this);
  vLayout1->addWidget(l_);

  numPoints_ = 16;
  dataPoints_ = new double[numPoints_];
}

RealWidget::~RealWidget()
{
  delete l_;
}

void RealWidget::customEvent( QEvent * e )
{
  if(e->type() == RealDataEvent::type)
  {
    RealDataEvent* dataEvent = (RealDataEvent*)e;
    plotData(dataEvent);
  }
}

void RealWidget::plotData(RealDataEvent* e)
{
  if(e->numPoints_ != numPoints_)
  {
    numPoints_ = e->numPoints_;
    delete [] dataPoints_;
    dataPoints_ = new double[numPoints_];
  }
  for(int i=0;i<numPoints_;i++)
    dataPoints_[i] = e->dataPoints_[i];

  l_->plotData(dataPoints_, numPoints_);
}

void RealWidget::setWidgetTitle(QString title)
{
  l_->setTitle(title);
}

void RealWidget::setWidgetAxisLabels(QString xLabel, QString yLabel)
{
  l_->setXLabel(xLabel);
  l_->setYLabel(yLabel);
}

void RealWidget::setWidgetAxes(double xMin, double xMax,
                               double yMin, double yMax)
{
  l_->setAxes(xMin, xMax, yMin, yMax);
}

void RealWidget::setWidgetXAxisRange(double xMin, double xMax)
{
  l_->setXAxisRange(xMin, xMax);
}
