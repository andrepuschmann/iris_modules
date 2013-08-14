#include "ComplexplotWrapper.h"

#include "ComplexWidget.h"
#include "graphics/qt/common/Events.h"
#include <qapplication.h>
#include <QThread>

using namespace std;


ComplexplotWrapper::ComplexplotWrapper()
    :widget_(NULL)
{
  if(QCoreApplication::instance() == NULL)
    return; //TODO: throw exception here in Iris
  if(QCoreApplication::instance()->thread() == QThread::currentThread())
  {
    connect( this, SIGNAL( createWidgetSignal() ), this, SLOT(createWidgetSlot()) );
    connect( this, SIGNAL( destroyWidgetSignal() ), this, SLOT(destroyWidgetSlot()) );
  }
  else
  {
    connect( this, SIGNAL( createWidgetSignal() ), this, SLOT(createWidgetSlot()), Qt::BlockingQueuedConnection );
    connect( this, SIGNAL( destroyWidgetSignal() ), this, SLOT(destroyWidgetSlot()), Qt::BlockingQueuedConnection );
    moveToThread(QCoreApplication::instance()->thread());
  }
  emit createWidgetSignal();
}

ComplexplotWrapper::~ComplexplotWrapper()
{
  emit destroyWidgetSignal();
}

void ComplexplotWrapper::createWidgetSlot()
{
  widget_ = new ComplexWidget;
  connect(this, SIGNAL(setWidgetTitle(QString)),
          widget_, SLOT(setWidgetTitle(QString)));
  connect(this, SIGNAL(setWidgetXAxisScale(int,double,double)),
          widget_, SLOT(setWidgetXAxisScale(int,double,double)));
  connect(this, SIGNAL(setWidgetYAxisScale(int,double,double)),
          widget_, SLOT(setWidgetYAxisScale(int,double,double)));
  connect(this, SIGNAL(setWidgetXAxisAutoScale(int,bool)),
          widget_, SLOT(setWidgetXAxisAutoScale(int,bool)));
  connect(this, SIGNAL(setWidgetYAxisAutoScale(int,bool)),
          widget_, SLOT(setWidgetYAxisAutoScale(int,bool)));
  connect(this, SIGNAL(setWidgetXAxisRange(double,double)),
          widget_, SLOT(setWidgetXAxisRange(double,double)));

  widget_->resize( 800, 600 );
  widget_->show();
}

void ComplexplotWrapper::destroyWidgetSlot()
{
  delete widget_;
}

void ComplexplotWrapper::setNewData(complex<double>* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new ComplexDataEvent(data, numPoints));
}

void ComplexplotWrapper::setNewData(complex<float>* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new ComplexDataEvent(data, numPoints));
}

void ComplexplotWrapper::setTitle(std::string title)
{
  if(widget_ == NULL)
    return;
  QString str = QString::fromUtf8(title.c_str());
  emit setWidgetTitle(str);
}

void ComplexplotWrapper::setXAxisAutoScale(int id, bool on=true)
{
  if(widget_ == NULL)
    return;
  emit setWidgetXAxisAutoScale(id, on);
}

void ComplexplotWrapper::setYAxisAutoScale(int id, bool on=true)
{
  if(widget_ == NULL)
    return;
  emit setWidgetYAxisAutoScale(id, on);
}

void ComplexplotWrapper::setXAxisScale(int id, double xMin, double xMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetXAxisScale(id, xMin, xMax);
}

void ComplexplotWrapper::setYAxisScale(int id, double yMin, double yMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetYAxisScale(id, yMin, yMax);
}

void ComplexplotWrapper::setXAxisRange(double xMin, double xMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetXAxisRange(xMin, xMax);
}

