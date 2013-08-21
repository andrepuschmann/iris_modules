#include "WaterfallplotWrapper.h"

#include "WaterfallWidget.h"
#include "graphics/qt/common/Events.h"
#include <qapplication.h>
#include <QThread>

using namespace std;

WaterfallplotWrapper::WaterfallplotWrapper(int numDataPoints, int numRows)
    :widget_(NULL)
    ,destroyed_(true)
{
  if(QCoreApplication::instance() == NULL)
    return; //TODO: throw exception here in Iris
  if(QCoreApplication::instance()->thread() == QThread::currentThread())
  {
    connect( this, SIGNAL( createWidgetSignal(int, int) ),
             this, SLOT(createWidgetSlot(int, int)) );
    connect( this, SIGNAL( destroyWidgetSignal() ),
             this, SLOT(destroyWidgetSlot()) );
    connect( this, SIGNAL( destroyWidgetSignalBlocking() ),
             this, SLOT(destroyWidgetSlot()) );
  }
  else
  {
    connect( this, SIGNAL( createWidgetSignal(int, int) ),
             this, SLOT(createWidgetSlot(int, int)),
             Qt::BlockingQueuedConnection );
    connect( this, SIGNAL( destroyWidgetSignal() ),
             this, SLOT(destroyWidgetSlot()) );
    connect( this, SIGNAL( destroyWidgetSignalBlocking() ),
             this, SLOT(destroyWidgetSlot()),
             Qt::BlockingQueuedConnection );
    moveToThread(QCoreApplication::instance()->thread());
  }
  emit createWidgetSignal(numDataPoints, numRows);
}

WaterfallplotWrapper::~WaterfallplotWrapper()
{
  if(destroyed_)
    emit destroyWidgetSignal();
  else
    emit destroyWidgetSignalBlocking();
}

void WaterfallplotWrapper::createWidgetSlot(int numDataPoints, int numRows)
{
  widget_ = new WaterfallWidget(numDataPoints, numRows);
  destroyed_ = false;
  widget_->setAttribute(Qt::WA_DeleteOnClose, true);
  connect(widget_, SIGNAL( destroyed() ),
          this, SLOT( widgetDestroyed() ));
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

void WaterfallplotWrapper::widgetDestroyed()
{
  destroyed_ = true;
}

void WaterfallplotWrapper::appendNewData(float* data, int numPoints)
{
  if(destroyed_)
    return;
  qApp->postEvent(widget_, new RealDataEvent(data, numPoints));
}

void WaterfallplotWrapper::appendNewData(double* data, int numPoints)
{
  if(destroyed_)
    return;
  qApp->postEvent(widget_, new RealDataEvent(data, numPoints));
}


void WaterfallplotWrapper::setTitle(std::string title)
{
  if(destroyed_)
    return;
  QString str = QString::fromUtf8(title.c_str());
  emit setWidgetTitle(str);
}

void WaterfallplotWrapper::setPlotXAxisScale(double xMin, double xMax)
{
  if(destroyed_)
    return;
  emit setWidgetPXAxisScale(xMin, xMax);
}

void WaterfallplotWrapper::setSpectrogramXAxisScale(double xMin, double xMax)
{
  if(destroyed_)
    return;
  emit setWidgetSXAxisScale(xMin, xMax);
}

void WaterfallplotWrapper::setPlotYAxisScale(double yMin, double yMax)
{
  if(destroyed_)
    return;
  emit setWidgetPYAxisScale(yMin, yMax);
}

void WaterfallplotWrapper::setSpectrogramYAxisScale(double yMin, double yMax)
{
  if(destroyed_)
    return;
  emit setWidgetSYAxisScale(yMin, yMax);
}

void WaterfallplotWrapper::setSpectrogramZAxisScale(double zMin, double zMax)
{
  if(destroyed_)
    return;
  emit setWidgetSZAxisScale(zMin, zMax);
}
