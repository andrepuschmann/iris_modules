#include "WaterfallWidget.h"
#include "graphics/qt/common/Spectrogramplot.h"
#include "graphics/qt/common/Lineplot.h"
#include "graphics/qt/common/Events.h"

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
  timerId_ = startTimer(10);
  haveNewData_ = false;
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
    appendData(dataEvent);
  }
}

void WaterfallWidget::setWidgetTitle(QString title)
{
  setWindowTitle(title);
}

void WaterfallWidget::setPlotXAxisScale(double xMin, double xMax)
{
  p_->setAxisScale(QwtPlot::xBottom, xMin, xMax);
}

void WaterfallWidget::setSpectrogramXAxisScale(double xMin, double xMax)
{
  s_->setPlotXAxisScale(xMin, xMax);
}

void WaterfallWidget::setPlotYAxisScale(double yMin, double yMax)
{
  p_->setAxisScale(QwtPlot::yLeft, yMin, yMax);
}

void WaterfallWidget::setSpectrogramYAxisScale(double yMin, double yMax)
{
  s_->setPlotYAxisScale(yMin, yMax);
}

void WaterfallWidget::setSpectrogramZAxisScale(double zMin, double zMax)
{
  s_->setPlotZAxisScale(zMin, zMax);
}

void WaterfallWidget::timerEvent(QTimerEvent *event)
{
  if(event->timerId() == timerId_)
  {
    if(haveNewData_)
    {
      p_->replot();
      s_->replot();
      haveNewData_ = false;
    }
    return;
  }
  QWidget::timerEvent(event);
}

void WaterfallWidget::appendData(RealDataEvent* e)
{
  if(e->numPoints_ != numPoints_)
  {
    numPoints_ = e->numPoints_;
    delete [] data_;
    data_ = new double[numPoints_];
  }

  memcpy(data_, e->dataPoints_, numPoints_*sizeof(double));

  p_->setData(data_, numPoints_);
  s_->appendData(data_, numPoints_);
  haveNewData_ = true;
}
