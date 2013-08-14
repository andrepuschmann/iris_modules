#include "WaterfallplotWrapper.h"

#include "WaterfallWidget.h"
#include "graphics/qt/common/Events.h"
#include <qapplication.h>
#include <QThread>

using namespace std;

WaterfallplotWrapper::WaterfallplotWrapper(int numDataPoints, int numRows)
    :widget_(NULL)
{
  if(QCoreApplication::instance() == NULL)
    return; //TODO: throw exception here in Iris
  if(QCoreApplication::instance()->thread() == QThread::currentThread())
  {
    connect( this, SIGNAL( createWidgetSignal(int, int) ),
             this, SLOT(createWidgetSlot(int, int)) );
    connect( this, SIGNAL( destroyWidgetSignal() ), this, SLOT(destroyWidgetSlot()) );
  }
  else
  {
    connect( this, SIGNAL( createWidgetSignal(int, int) ),
             this, SLOT(createWidgetSlot(int, int)),
             Qt::BlockingQueuedConnection );
    connect( this, SIGNAL( destroyWidgetSignal() ),
             this, SLOT(destroyWidgetSlot()),
             Qt::BlockingQueuedConnection );
    moveToThread(QCoreApplication::instance()->thread());
  }
  emit createWidgetSignal(numDataPoints, numRows);
}

WaterfallplotWrapper::~WaterfallplotWrapper()
{
  emit destroyWidgetSignal();
}

void WaterfallplotWrapper::createWidgetSlot(int numDataPoints, int numRows)
{
  widget_ = new WaterfallWidget(numDataPoints, numRows);
  connect(this, SIGNAL(setWidgetTitle(QString)),
          widget_, SLOT(setWidgetTitle(QString)));
  connect(this, SIGNAL(setWidgetPXAxisScale(double, double)),
          widget_, SLOT(setPlotXAxisScale(double, double)));
  connect(this, SIGNAL(setWidgetSXAxisScale(double, double)),
          widget_, SLOT(setSpectrogramXAxisScale(double, double)));
  connect(this, SIGNAL(setWidgetPYAxisScale(double, double)),
          widget_, SLOT(setPlotYAxisScale(double, double)));
  connect(this, SIGNAL(setWidgetSYAxisScale(double, double)),
          widget_, SLOT(setSpectrogramYAxisScale(double, double)));
  connect(this, SIGNAL(setWidgetSZAxisScale(double, double)),
          widget_, SLOT(setSpectrogramZAxisScale(double, double)));

  widget_->resize( 800, 600 );
  widget_->show();
}

void WaterfallplotWrapper::destroyWidgetSlot()
{
  delete widget_;
}

void WaterfallplotWrapper::appendNewData(float* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new RealDataEvent(data, numPoints));
}

void WaterfallplotWrapper::appendNewData(double* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new RealDataEvent(data, numPoints));
}


void WaterfallplotWrapper::setTitle(std::string title)
{
  if(widget_ == NULL)
    return;
  QString str = QString::fromUtf8(title.c_str());
  emit setWidgetTitle(str);
}

void WaterfallplotWrapper::setPlotXAxisScale(double xMin, double xMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetPXAxisScale(xMin, xMax);
}

void WaterfallplotWrapper::setSpectrogramXAxisScale(double xMin, double xMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetSXAxisScale(xMin, xMax);
}

void WaterfallplotWrapper::setPlotYAxisScale(double yMin, double yMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetPYAxisScale(yMin, yMax);
}

void WaterfallplotWrapper::setSpectrogramYAxisScale(double yMin, double yMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetSYAxisScale(yMin, yMax);
}

void WaterfallplotWrapper::setSpectrogramZAxisScale(double zMin, double zMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetSZAxisScale(zMin, zMax);
}
