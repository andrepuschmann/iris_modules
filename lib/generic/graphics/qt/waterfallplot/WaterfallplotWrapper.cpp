#include "WaterfallplotWrapper.h"

#include "WaterfallWidget.h"
#include "common/Events.h"
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
  connect(this, SIGNAL(setWidgetAxes(double, double, double, double, double, double)),
          widget_, SLOT(setPlotAxes(double, double, double, double, double, double)));

  widget_->resize( 800, 600 );
  widget_->show();
  widget_->moveToThread(QApplication::instance()->thread());
}

void WaterfallplotWrapper::destroyWidgetSlot()
{
  delete widget_;
}

void WaterfallplotWrapper::plotNewData(float* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new RealDataEvent(data, numPoints));
}

void WaterfallplotWrapper::plotNewData(double* data, int numPoints)
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

void WaterfallplotWrapper::setAxes(double xMin, double xMax,
                                   double yMin, double yMax,
                                   double zMin, double zMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetAxes(xMin, xMax, yMin, yMax, zMin, zMax);
}
