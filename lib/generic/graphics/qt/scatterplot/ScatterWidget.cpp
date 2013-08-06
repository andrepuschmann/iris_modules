#include "ScatterWidget.h"
#include "common/Pointplot.h"
#include "common/Events.h"

#include <qlayout.h>
#include <algorithm>
#include <boost/lambda/lambda.hpp>

using namespace std;

ScatterWidget::ScatterWidget(QWidget *parent)
  :QWidget(parent)
{
  plot_ = new Pointplot();
  QVBoxLayout* vLayout1 = new QVBoxLayout(this);
  vLayout1->addWidget(plot_);

  numPoints_ = 16;
  iData_ = new double[numPoints_];
  qData_ = new double[numPoints_];
}

ScatterWidget::~ScatterWidget()
{
  delete iData_;
  delete qData_;
}

void ScatterWidget::customEvent( QEvent * e )
{
  if(e->type() == ComplexDataEvent::type)
  {
    ComplexDataEvent* dataEvent = (ComplexDataEvent*)e;
    plotData(dataEvent);
  }
}

void ScatterWidget::plotData(ComplexDataEvent* e)
{
  if(e->numPoints_ != numPoints_)
  {
    numPoints_ = e->numPoints_;
    delete [] iData_;
    delete [] qData_;
    iData_ = new double[numPoints_];
    qData_ = new double[numPoints_];
  }

  transform(e->dataPoints_, &e->dataPoints_[numPoints_], iData_, opReal());
  transform(e->dataPoints_, &e->dataPoints_[numPoints_], qData_, opImag());

  plot_->plotData(iData_, qData_, numPoints_);
}

void ScatterWidget::setWidgetTitle(QString title)
{
  plot_->setTitle(title);
}

void ScatterWidget::setWidgetAxisLabels(QString xLabel, QString yLabel)
{
  plot_->setXLabel(xLabel);
  plot_->setYLabel(yLabel);
}

void ScatterWidget::setWidgetAxes(double xMin, double xMax,
                               double yMin, double yMax)
{
  plot_->setAxes(xMin, xMax, yMin, yMax);
}
